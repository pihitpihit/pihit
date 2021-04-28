#include <auto_prime.h>
using namespace Plastics;
using namespace std;

AutoFinalizer::AutoFinalizer( std::function<void()> finalizer ):
	finalizer_( finalizer )
{
}

AutoFinalizer::~AutoFinalizer()
{
	finalizer_();
}

