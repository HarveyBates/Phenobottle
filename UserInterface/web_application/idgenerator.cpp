#include <iostream>
#include <string>

static const char characters[] = "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*abcdefghijklmnopqrstuvwxyz";
static unsigned int length = sizeof(characters) - 1;

char generateId(){
		return characters[rand() % length];
}

int main(void)
{
	srand(time(0));
	std::string phenobottleID;
	for(int i = 0; i < 20; i++){
		phenobottleID += generateId();
	}
	std::cout << phenobottleID << std::endl;
	return 0;
}

