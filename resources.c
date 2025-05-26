// Make use of C23 #embed directive to quickly include resources.
// Resources are embedded directly in the file as binary data.

#include <stddef.h>

const unsigned char MASCOT[]= {
    #embed "data/mascot.png"
};
const size_t MASCOT_SIZE = sizeof(MASCOT);
