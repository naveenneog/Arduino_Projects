#include <unistd.h>
#include <iostream>
#include <signal.h>
#include "ublox6.h"

using namespace std;

bool shouldRun = true;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}

const size_t bufferLength = 256;

int main (int argc, char **argv)
{
  signal(SIGINT, sig_handler);

//! [Interesting]
  // Instantiate a Ublox6 GPS device on uart 0.
  upm::Ublox6* nmea = new upm::Ublox6(0);

  // make sure port is initialized properly.  9600 baud is the default.
  if (!nmea->setupTty(B9600))
    {
      cerr << "Failed to setup tty port parameters" << endl;
      return 1;
    }

  // Collect and output NMEA data.  There are various libraries out on
  // the Internet, such as tinyGPS or tinyGPS++ that can handle
  // decoding NMEA data and presenting it in a more easily accessible
  // format.  This example will just check for, and read raw NMEA data
  // from the device and output it on stdout.

  // This device also supports numerous configuration options, which
  // you can set with writeData().  Please refer to the Ublox-6 data
  // sheet for further information on the formats of the data sent and
  // received, and the various operating modes available.

  char nmeaBuffer[bufferLength];
  while (shouldRun)
    {
      // we don't want the read to block in this example, so always
      // check to see if data is available first.
      if (nmea->dataAvailable())
        {
          int rv = nmea->readData(nmeaBuffer, bufferLength);

          if (rv > 0)
            write(1, nmeaBuffer, rv);

          if (rv < 0) // some sort of read error occured
            {
              cerr << "Port read error." << endl;
              break;
            }

          continue;
        }

      usleep(100000); // 100ms
    }
//! [Interesting]

  cout << "Exiting..." << endl;

  delete nmea;
  return 0;
}

