#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<cmath>
#include<chrono>
#include<memory>
#include"Camera.h"
#include"Pixel.h"
#include"Screen.h"
#include"Obstacles.h"
#include"SDL2/SDL.h"
#include"fast_inverse_square_root.h"
#include<omp.h>

int main(){

    auto start_time = std::chrono::high_resolution_clock::now();

    int screen_width = 1920;
    int screen_height = 1080;
    int Frame = 0;
    int n_screen_pixels = screen_width;//can be changed to higher resolution by increasing screen dimensions
    
    Camera e(-0.12447,-0.12447); //coordinates of camera
    Screen S(-0.1,0.0,0.0,-0.1,n_screen_pixels); //Screen
    e.Set_Camera_Normal((S.get_x1() + S.get_x2())/2.,(S.get_y1() + S.get_y2())/2.);
    
    //Defining Obstacles
    //Can be modified at run time
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    obstacles.push_back(std::make_unique<Wall>(1.0,'H'));
    obstacles.push_back(std::make_unique<Wall>(-1.0,'H'));
    obstacles.push_back(std::make_unique<Wall>(0.5,'V'));
    obstacles.push_back(std::make_unique<Wall>(-0.5,'V'));
    obstacles.push_back(std::make_unique<Square>(0.05,0.05,0.1));
    obstacles.push_back(std::make_unique<Circle>(-0.17,0.4,0.2));

    //Pixel parameters
    Pixel p[n_screen_pixels];
    std::ofstream output("Frame_Rate_Log.txt",std::ios_base::app);
    
    auto current_time = std::chrono::system_clock::now();
    std::time_t date_time = std::chrono::system_clock::to_time_t(current_time);

    output<<"\nFrame Rate Log: "<<std::ctime( &date_time);
    
    SDL_Window *window;                    // Declare a pointer

    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

    // Create an application window with the following settings
    window = SDL_CreateWindow(
        "2D Ray Tracer",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        screen_width,                      // width, in pixels
        screen_height,                     // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_RenderClear(renderer);  

    int n = n_screen_pixels;
    SDL_Rect rect[n][3];

    
    double Sx1,Sx2,Sy1,Sy2;//temp variables for screen coordinates
    double ex,ey;//temp variables for camera coordinates
    double eNx,eNy,Nx,Ny;//temp variables for screen normal
    double Tx,Ty;//temp variables for vector along screen S1->S2
    double theta=0;//Rotation rate in radians (radians/frame)
    double dx,dy;//Translation in x,y directions per frame
    int h_obj, h_sky, h_ground;
    double px,py;

    std::vector<double> frame_log;//vector to store the frame times in milliseconds
    bool is_game_running = true;
    while(is_game_running){
        auto start = std::chrono::high_resolution_clock::now();
        //Uncomment for testing FPS performance:
        if(Frame>220)
            break;
            

        ex = e.get_x();
        ey = e.get_y();

        Sx1 = S.get_x1();
        Sy1 = S.get_y1();

        Sx2 = S.get_x2();
        Sy2 = S.get_y2();

        eNx = e.get_nX();
        eNy = e.get_nY();

        //N vector in direction normal to screen
        
        double inv_e_Nnorm = inverse_rsqrt( eNx*eNx + eNy*eNy); //inverse of the norm of Normal vector to Screen
        Nx = eNx * inv_e_Nnorm;
        Ny = eNy * inv_e_Nnorm;
        

        //T vector in direction of the screen (S.1 -> S.2)
        
        double inv_S_Tnorm = inverse_rsqrt((Sx2 - Sx1)*(Sx2 - Sx1) + (Sy2 - Sy1)*(Sy2 - Sy1));
        Tx = (Sx2 - Sx1)*inv_S_Tnorm;
        Ty = (Sy2 - Sy1)*inv_S_Tnorm;

        SDL_Event event;
        SDL_SetRelativeMouseMode(SDL_TRUE);
        while(SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_QUIT:
                    is_game_running = false;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym){
                        case SDLK_w:
                        //Move forward (along the direction of screen normal)
                            dx = 0.001*Nx;
                            dy = 0.001*Ny;
                            break;
                        case SDLK_s:
                        //Move Backward (opposite the screen normal)
                            dx = - 0.001*Nx;
                            dy = - 0.001*Ny;
                            break;
                        case SDLK_a:
                        //Move Left (opposite the vector along screen S1->S2)
                            dx = -0.001*Tx;
                            dy = -0.001*Ty;
                            break;
                        case SDLK_d:
                        //Move Right (along the direction of vector along screen)
                            dx = 0.001*Tx;
                            dy = 0.001*Ty;
                            break;
                        case SDLK_q:
                            is_game_running = false;
                            break;
                        }
                    break;   
                case SDL_MOUSEMOTION:{
                    if(event.motion.xrel < -0.01){
                        theta = -0.0628318;
                    }
                    else if(event.motion.xrel > 0.01){
                        theta = 0.0628318;
                    }
                    else
                        theta = 0;

                    //Rotating Screen about camera
                    Sx1 = (S.get_x1() - e.get_x()) * cos(theta) + (S.get_y1() - e.get_y()) * sin(theta) + e.get_x();
                    Sy1 = (S.get_x1() - e.get_x())*(-sin(theta))+ (S.get_y1() - e.get_y()) * cos(theta) + e.get_y();

                    Sx2 = (S.get_x2() - e.get_x()) * cos(theta) + (S.get_y2() - e.get_y()) * sin(theta) + e.get_x();
                    Sy2 = (S.get_x2() - e.get_x())*(-sin(theta))+ (S.get_y2() - e.get_y()) * cos(theta) + e.get_y();
                    theta = 0;
                    break;
                }
                case SDL_KEYUP:
                    switch (event.key.keysym.sym){
                        case SDLK_w:
                        case SDLK_a:
                        case SDLK_s:
                        case SDLK_d:
                            dx = 0;
                            dy = 0;
                            break;
                    }
                    break;
                default:
                    break;
            }

        }
    
        //Translating camera
        ex += dx;
        ey += dy;

        //Translating Screen
        Sx1 += dx;
        Sx2 += dx;

        Sy1 += dy;
        Sy2 += dy;
        
        //Assigning new coordinates to camera
        e.Set_Coordinates(ex,ey);
        
        //Assigning new coordinates to screen 
        S.Set_Coordinates(Sx1,Sy1,Sx2,Sy2);
        
        //Assigning new normal to camera for changed screen coordinates
        e.Set_Camera_Normal( (Sx1 + Sx2)/2. , (Sy1 + Sy2)/2.);

        omp_set_num_threads(4);
        //Compute Screen Pixel coordinates for changed screen coordinates
        #pragma omp parallel for schedule(static) private(px,py)
            for(int i=0;i<n_screen_pixels;i++){
                //int thread = omp_get_thread_num();
                //std::cout<<"Thread: "<<thread<<std::endl;
                px = Sx1 * (1. - static_cast<double>(i)/n_screen_pixels - 1./(2 * n_screen_pixels)); 
                px += Sx2 * ( static_cast<double>(i)/n_screen_pixels + 1./(2 * n_screen_pixels));
                py = Sy1 * (1. - static_cast<double>(i)/n_screen_pixels - 1./(2 * n_screen_pixels));
                py += Sy2 * ( static_cast<double>(i)/n_screen_pixels + 1./(2 * n_screen_pixels));
                //std::cout<<"Pixel "<<i<<" Position:"<<p_x<<" ,"<<p_y<<std::endl;
                p[i].Set_Position(px,py);
            }
        eNx = e.get_nX();
        eNy = e.get_nY();
        //1D Screen Distance Calculator Loop
        //For each pixel find intersection of ray of light starting from camera to the pixel and all obstacles
        //If intersection exists find the ray parameter t and compute distance between point of intersection of obstacle
        //and the pixel along the screen normal
        //Store the distance in the screen container for that pixel
        omp_set_num_threads(4);
        #pragma omp parallel for schedule(static)
            for(int i=0;i<n_screen_pixels;i++){
                
                double min_dist = 1e6;
                char obstacle_type;
                double dist_;
                
                //Computing Intersection with all obstacles for ray 
                //starting from the camera passing through pixel "i"
                std::pair<bool,double> intersect_pair;
                
                for(int k=0;k<obstacles.size();k++){
                    intersect_pair = obstacles[k]->Compute_Intersection(e,p[i]);
                    if(intersect_pair.first){
                        dist_ = (intersect_pair.second * (p[i].get_x() - ex)) * eNx +
                        (intersect_pair.second * (p[i].get_y() - ey)) * eNy;
                        //dist_ *= inverse_rsqrt(eNx*eNx + eNy*eNy);
                        dist_ = std::fabs(dist_);
                        if(min_dist > dist_){
                            min_dist = dist_;
                            obstacle_type = obstacles[k]->Obstacle_type();
                        }        
                    }
                }
                //Append ith pixel information (minimum intersection distance and obstacle encountered)
                //to Screen object S
                S.Append_to_Screen(i,min_dist,obstacle_type);
            }
        
        //Computing FPS here to accurately compute speed in Computing Frame due to 
        //parallelization as main bottleneck is still frame rendering(which cannot be parallelized further)

        //Save time elapsed for frame in the log vector
        auto stop = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = stop - start;
        frame_log.push_back(diff.count());

        Frame++;

        //Render the frame
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (Frame==0)
            SDL_RenderClear(renderer);
        int rect_w = screen_width/n;
        int N = S.screen.size();
        omp_set_num_threads(4);
        #pragma omp parallel for schedule(static,1) private(h_obj,h_sky,h_ground)
            for(int i=0;i<N;i++){
                
                //Height of rectangles at that pixel 
                //h_obj = screen_height * sqrt(eNx * eNx + eNy * eNy)/S.screen[i].first;
                h_obj = screen_height * (eNx * eNx + eNy * eNy)/S.screen[i].first;
                h_sky = (screen_height - h_obj)/2;
                h_ground = h_sky;

                
                //rect[i][0] -> sky
                //rect[i][1] -> object
                //rect[i][2] -> ground
                rect[i][0].w = rect_w;//n : No. of pixels in the screen
                rect[i][0].h = h_sky;
                rect[i][0].x = i * rect_w;
                rect[i][0].y = 0;
                
                rect[i][1].w = rect_w;
                rect[i][1].h = h_obj;
                rect[i][1].x = i * rect_w;
                rect[i][1].y = h_sky;

                
                rect[i][2].w = rect_w;
                rect[i][2].h = h_ground;
                rect[i][2].x = i * rect_w;
                rect[i][2].y = h_sky + h_obj;

                #pragma omp critical
                {
                    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
                    SDL_RenderFillRect(renderer, &rect[i][0]);
                
                
                    switch (S.screen[i].second){
                        case 'W':
                            SDL_SetRenderDrawColor(renderer, 110, 110, 110, 255);
                            break;
                
                        case 'S':
                            SDL_SetRenderDrawColor(renderer, 165, 42, 42, 255);
                            break;
                
                        case 'C':
                            SDL_SetRenderDrawColor(renderer, 125, 252, 0, 255);
                    }
                
                    SDL_RenderFillRect(renderer, &rect[i][1]);
                
                    SDL_SetRenderDrawColor(renderer, 107, 66, 38, 255);
                    SDL_RenderFillRect(renderer, &rect[i][2]);
                }
                
            }

        SDL_RenderPresent(renderer);
        
    }

    //Close the program after SDL_Quit
    SDL_DestroyWindow(window);

    SDL_Quit();

    auto end_time = std::chrono::high_resolution_clock::now();
    
    //Writing Frame Loading Time in log file
    double total_frame_time=0;
    #pragma omp parallel for ordered schedule(static) reduction(+:total_frame_time)
    for(int frame=1;frame<frame_log.size();frame++){
        #pragma omp ordered
            output<<"Frame "<<frame<<" : "<<frame_log[frame]<<" seconds\n";
        total_frame_time+= frame_log[frame];
    }
    
    auto total_program_duration = std::chrono::duration_cast<std::chrono::microseconds> (end_time - start_time);
    output<<"Total Program Time:"<<total_program_duration.count()*1e-6<<" seconds\n";
    output<<"Avg FPS: "<<frame_log.size()/total_frame_time<<"\n";
    output.close();
    return 0;
}