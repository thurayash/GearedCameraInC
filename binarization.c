void binarization(SDL_Surface *rob, SDL_Surface *skin)
{
    Uint32 pixel;
    Uint8 r, g, b;


    for (int i = 1; i <rob->w; i++)
    {
        for (int j = 1; j <rob->h; j++)
        {
            //conv treshold. if we have 7 or 8 surrounding red pixels, then take the pixel as red
            int convolution = 0;
            pixel = get_pixel(rob, i, j);

            SDL_GetRGB(pixel, rob->format, &r , &g, &b);

            if(r ==255)
            {
                Uint32 pixel1;
                Uint8 r1, g1, b1;

                pixel1 = get_pixel(rob, i, j+1);
                SDL_GetRGB(pixel1, rob->format, &r1 , &g1, &b1);
                if(r1 ==255)
                    convolution+=1;

                pixel1 = get_pixel(rob, i, j);
                SDL_GetRGB(pixel, rob->format, &r1 , &g1, &b1);
                if(r1 ==255)
                    convolution+=1;

                pixel1 = get_pixel(rob, i, j-1);
                SDL_GetRGB(pixel, rob->format, &r1 , &g1, &b1);
                if(r1 ==255)
                    convolution+=1;

                pixel1 = get_pixel(rob, i-1, j);
                SDL_GetRGB(pixel, rob->format, &r1 , &g1, &b1);
                if(r1 ==255)
                    convolution+=1;

                pixel1 = get_pixel(rob, i, j);
                SDL_GetRGB(pixel, rob->format, &r1 , &g1, &b1);
                if(r1 ==255)
                    convolution+=1;

                pixel1 = get_pixel(rob, i+1, j);
                SDL_GetRGB(pixel, rob->format, &r1 , &g1, &b1);
                if(r1 ==255)
                    convolution+=1;

                pixel1 = get_pixel(rob, i-1, j-1);
                SDL_GetRGB(pixel, rob->format, &r1 , &g1, &b1);
                if(r1 ==255)
                    convolution+=1;

                pixel1 = get_pixel(rob, i+1, j+1);
                SDL_GetRGB(pixel, rob->format, &r1 , &g1, &b1);
                if(r1 ==255)
                    convolution+=1;
            }
            if (convolution>=7)
            {
                pixel = SDL_MapRGB(skin->format, 255, 0, 0);
            
                put_pixel(skin, i,j, pixel);
            }
        }
    }
}