/*
 *  «FILENAME»
 *  «PROJECTNAME»
 *
 *  Created by «FULLUSERNAME» on «DATE».
 *  Copyright «YEAR» «ORGANIZATIONNAME». All rights reserved.
 *
 */


«OPTIONALHEADERINCLUDELINE»

«FILEBASENAMEASIDENTIFIER»::«FILEBASENAMEASIDENTIFIER»()
{
	scale = 0.0;
}

//This method is called every frame, for the particle
void «FILEBASENAMEASIDENTIFIER»::onDraw(float timestep)
{
	//Rotate the object
	position.y += timestep;
	scale += 1.0*timestep;
	color.w -= 0.3*timestep;
	
	if(color.w <= 0.0)
		destroy = true;
}
