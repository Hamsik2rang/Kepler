#include <Kepler.h>

class Client : public kepler::Application
{
public:
	Client()
	{

	}
	~Client()
	{

	}
};

kepler::Application* kepler::CreateApplication()
{
	return new Client();
}

int main()
{


	return 0;
}