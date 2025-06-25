
#include "RuntimeManager.hpp"
#include "Compute.hpp"

int main( int argc, char *argv[] ) {

    if ( argc )
        std::cout << "\nRunning program: " << argv[0] << std::endl << std::endl;

    try {
        int error = run_main_routine();
        if ( error ) return EXIT_FAILURE;
    } catch( const std::exception & e ) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
