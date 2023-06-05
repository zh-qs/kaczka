#include "application.h"

using namespace kaczka;

int main() 
{
	Application app(1280, 720, "Kaczka", { 0.6196f,0.7451f,0.8431f,1.0f });
	app.run();
	return 0;
}