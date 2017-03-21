#version 330 core

uniform sampler2D tex;  //the input texture
in vec2 v_uv;

out vec4 fragColor;
out vec4 fragColor2;

float scale = 1.0/768.0;
float getX()
{
    return texture(tex, v_uv).x;
}

float getY()
{
    return texture(tex, v_uv).y;
}
 
float getZ()
{
    return texture(tex, v_uv).z;
}

float getVec(vec2 vector)
{
    return texture(tex, v_uv+vector*scale).x;
}

float getVecZ(vec2 vector)
{
    return texture(tex, v_uv+vector*scale).z;
}

void main() 
{
    vec2[4] directions;
    directions[0] = vec2(1,0);		//right
    directions[1] = vec2(0,-1);		//down
    directions[2] = vec2(-1,0);		//left
    directions[3] = vec2(0,1);		//up

    vec2[4] surrounding;
    surrounding[0] = vec2(1,1);		//up right
    surrounding[1] = vec2(1,-1);	//down right
    surrounding[2] = vec2(-1,-1);	//down left
    surrounding[3] = vec2(-1,1);	//up left

    bool isSet = false;
    if(getX() > 0.5 && getX() < 0.7) ///poprzedni
    {
        if(getY()>0.9)
        {
			if( getZ() > 0.9 )
				fragColor = vec4(0.4,0.4,1.0,1.0);
			else if( getZ() < 0.1 )
				fragColor = vec4(0.4,0.4,0.0,1.0);
			else
				fragColor = vec4(0.4,0.4,0.5,1.0);
        }
        else
        {
            fragColor = vec4(0.0,0.0,0.0,1.0);
        }
        isSet = true;
    }

    if(getX() > 0.7 ) // glowa
    {
		if( getZ() > 0.9 )
			fragColor = vec4(0.6,getY(),1.0,1.0);
		else if( getZ() < 0.1 )
			fragColor = vec4(0.6,getY(),0.0,1.0);
		else
			fragColor = vec4(0.6,getY(),0.5,1.0);	
        isSet = true;
    }

    if(!isSet)
    {
        bool isHeadNear = false;
        for(int i = 0 ; i < 4 ; i++)
        {
            if(getVec(directions[i]) > 0.7)
            {
                if(getVec(directions[i])> 0.9)
                {
                    if(getVec(surrounding[(i+1)%4]) > 0.5 && getVec(surrounding[(i+1)%4])  < 0.7)
                    {
                        if(getX() < 0.1 )		//jesli bylo czarne
                        {
							if(getVecZ(surrounding[(i+1)%4]) > 0.9)
								fragColor = vec4(1.0,1.0,1.0,1.0);
							else if(getVecZ(surrounding[(i+1)%4]) < 0.1)
								fragColor = vec4(1.0,1.0,0.0,1.0);
							else
								fragColor = vec4(1.0,1.0,0.5,1.0);
						}
                        else					//jesli bylo inne
                        {
							if(getVecZ(surrounding[(i+1)%4]) > 0.9)
								fragColor = vec4(0.8,0.8,1.0,1.0);
							else if(getVecZ(surrounding[(i+1)%4]) < 0.1)
								fragColor = vec4(0.8,0.8,0.0,1.0);
							else
								fragColor = vec4(0.8,0.8,0.5,1.0);
                        }
                    }
                    else
                    {
                        fragColor = texture(tex, v_uv);
                    }
                }
                else
                {
                    if(getVec(surrounding[i]) > 0.5 && getVec(surrounding[i]) < 0.7)
                    {
                        if(getX() < 0.1 )		//jesli bylo czarne
                        {
							if(getVecZ(surrounding[i]) > 0.9)
								fragColor = vec4(1.0,1.0,1.0,1.0);
							else if(getVecZ(surrounding[i]) < 0.1)
								fragColor = vec4(1.0,1.0,0.0,1.0);
							else
								fragColor = vec4(1.0,1.0,0.5,1.0);
                        }
                        else					//jesli bylo inne
                        {
							if(getVecZ(surrounding[i]) > 0.9)
								fragColor = vec4(0.8,0.8,1.0,1.0);
							else if(getVecZ(surrounding[i]) < 0.1)
								fragColor = vec4(0.8,0.8,0.0,1.0);
							else
								fragColor = vec4(0.8,0.8,0.5,1.0);
						}
                    }
                    else
                    {
                        fragColor = texture(tex, v_uv);
                    }
                }
                isHeadNear = true;
                break;
            }
        }
        if(!isHeadNear)
        {
            fragColor = texture(tex, v_uv);
        }
    }
}