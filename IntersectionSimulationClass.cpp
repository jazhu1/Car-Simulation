// James Zhu
// 12/3/2024
// Handles traffic simulation at a four-way intersection. The simulation
// handles events such as car arrivals and traffic light changes, 
// tracks queue lengths, and statistical data.

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

#include "IntersectionSimulationClass.h"
#include "random.h"

void IntersectionSimulationClass::readParametersFromFile(
     const string &paramFname
     )
{
  bool success = true;;
  ifstream paramF;

  paramF.open(paramFname.c_str());

  //Check that the file was able to be opened...
  if (paramF.fail())
  {
    success = false;
    cout << "ERROR: Unable to open parameter file: " << paramFname << endl;
  }
  else
  {
    //Now read in all the params, according to the specified format of
    //the text-based parameter file.
    if (success)
    {
      paramF >> randomSeedVal;
      if (paramF.fail() ||
          randomSeedVal < 0)
      {
        success = false;
        cout << "ERROR: Unable to read/set random generatsor seed" << endl;
      }
    }

    if (success)
    {
      paramF >> timeToStopSim;
      if (paramF.fail() ||
          timeToStopSim <= 0)
      {
        success = false;
        cout << "ERROR: Unable to read/set simulation end time" << endl;
      }
    }

    if (success)
    {
      paramF >> eastWestGreenTime >> eastWestYellowTime;
      if (paramF.fail() ||
          eastWestGreenTime <= 0 ||
          eastWestYellowTime <= 0)
      {
        success = false;
        cout << "ERROR: Unable to read/set east-west times" << endl;
      }
    }

    if (success)
    {
      paramF >> northSouthGreenTime >> northSouthYellowTime;
      if (paramF.fail() ||
          northSouthGreenTime <= 0 ||
          northSouthYellowTime <= 0)
      {
        success = false;
        cout << "ERROR: Unable to read/set north-south times" << endl;
      }
    }

    if (success)
    {
      paramF >> eastArrivalMean >> eastArrivalStdDev;
      if (paramF.fail() ||
          eastArrivalMean <= 0 ||
          eastArrivalStdDev < 0)
      {
        success = false;
        cout << "ERROR: Unable to read/set east arrival distribution" << endl;
      }
    }

    if (success)
    {
      paramF >> westArrivalMean >> westArrivalStdDev;
      if (paramF.fail() ||
          westArrivalMean <= 0 ||
          westArrivalStdDev < 0)
      {
        success = false;
        cout << "ERROR: Unable to read/set west arrival distribution" << endl;
      }
    }

    if (success)
    {
      paramF >> northArrivalMean >> northArrivalStdDev;
      if (paramF.fail() ||
          northArrivalMean <= 0 ||
          northArrivalStdDev < 0)
      {
        success = false;
        cout << "ERROR: Unable to read/set north arrival distribution" << endl;
      }
    }

    if (success)
    {
      paramF >> southArrivalMean >> southArrivalStdDev;
      if (paramF.fail() ||
          southArrivalMean <= 0 ||
          southArrivalStdDev < 0)
      {
        success = false;
        cout << "ERROR: Unable to read/set south arrival distribution" << endl;
      }
    }

    if (success)
    {
      paramF >> percentCarsAdvanceOnYellow;
      if (paramF.fail() || 
          percentCarsAdvanceOnYellow < 0 ||
          percentCarsAdvanceOnYellow > 100)
      {
        success = false;
        cout << "ERROR: Unable to read/set percentage yellow advance" << endl;
      }

      //Use the specified seed to seed the random number generator
      setSeed(randomSeedVal);
    }

    paramF.close();
  }

  //Let the caller know whether things went well or not by printing the
  if (!success)
  {
    cout << "ERROR: Parameter file was NOT read in successfully, so the " <<
            "simulation is NOT setup properly!" << endl;
    isSetupProperly = false;
  }
  else
  {
    cout << "Parameters read in successfully - simulation is ready!" << endl;
    isSetupProperly = true;
  }
}

void IntersectionSimulationClass::printParameters() const
{
  cout << "===== Begin Simulation Parameters =====" << endl;
  if (!isSetupProperly)
  {
    cout << "  Simulation is not yet properly setup!" << endl;
  }
  else
  {
    cout << "  Random generator seed: " << randomSeedVal << endl;
    cout << "  Simulation end time: " << timeToStopSim << endl;

    cout << "  East-West Timing -" <<
            " Green: " << eastWestGreenTime << 
            " Yellow: " << eastWestYellowTime <<
            " Red: " << getEastWestRedTime() << endl;

    cout << "  North-South Timing -" <<
            " Green: " << northSouthGreenTime << 
            " Yellow: " << northSouthYellowTime <<
            " Red: " << getNorthSouthRedTime() << endl;

    cout << "  Arrival Distributions:" << endl;
    cout << "    East - Mean: " << eastArrivalMean << 
            " StdDev: " << eastArrivalStdDev << endl;
    cout << "    West - Mean: " << westArrivalMean << 
            " StdDev: " << westArrivalStdDev << endl;
    cout << "    North - Mean: " << northArrivalMean << 
            " StdDev: " << northArrivalStdDev << endl;
    cout << "    South - Mean: " << southArrivalMean << 
            " StdDev: " << southArrivalStdDev << endl;

    cout << "  Percentage cars advancing through yellow: " <<
            percentCarsAdvanceOnYellow << endl;
  }
  cout << "===== End Simulation Parameters =====" << endl;
}

void IntersectionSimulationClass::scheduleArrival(const string &travelDir) {
    int arrivalMean = 0;
    int arrivalStdDev = 0;
    int eventType = 0;

    // Determine parameters based on travel direction
    if (travelDir == EAST_DIRECTION) {
        arrivalMean = eastArrivalMean;
        arrivalStdDev = eastArrivalStdDev;
        eventType = EVENT_ARRIVE_EAST;
    }
    else if (travelDir == WEST_DIRECTION) {
        arrivalMean = westArrivalMean;
        arrivalStdDev = westArrivalStdDev;
        eventType = EVENT_ARRIVE_WEST;
    }
    else if (travelDir == NORTH_DIRECTION) {
        arrivalMean = northArrivalMean;
        arrivalStdDev = northArrivalStdDev;
        eventType = EVENT_ARRIVE_NORTH;
    }
    else if (travelDir == SOUTH_DIRECTION) {
        arrivalMean = southArrivalMean;
        arrivalStdDev = southArrivalStdDev;
        eventType = EVENT_ARRIVE_SOUTH;
    }
    else {
        cout << "Error: Invalid travel direction!" << endl;
        return;
    }

    // Compute arrival time and event time
    int arrivalTime = getPositiveNormal(arrivalMean, arrivalStdDev);
    int eventTime = currentTime + arrivalTime;

    // Create and insert event
    EventClass singleEvent(eventTime, eventType);
    eventList.insertValue(singleEvent);

    // Output the scheduled event
    cout << "Time: " << currentTime 
         << " Scheduled " << singleEvent << endl;
}

void IntersectionSimulationClass::scheduleLightChange() {
    int eventTime = 0;
    int nextEventType = 0;
    int duration = 0;

    // Determine next event type and duration based on the current light
    if (currentLight == LIGHT_GREEN_EW) {
        nextEventType = EVENT_CHANGE_YELLOW_EW;
        duration = eastWestGreenTime;
    }
    else if (currentLight == LIGHT_YELLOW_EW) {
        nextEventType = EVENT_CHANGE_GREEN_NS;
        duration = eastWestYellowTime;
    }
    else if (currentLight == LIGHT_GREEN_NS) {
        nextEventType = EVENT_CHANGE_YELLOW_NS;
        duration = northSouthGreenTime;
    }
    else if (currentLight == LIGHT_YELLOW_NS) {
        nextEventType = EVENT_CHANGE_GREEN_EW;
        duration = northSouthYellowTime;
    }
    else {
        cout << "Error: Invalid light state!" << endl;
        return;
    }

    // Compute event time
    eventTime = currentTime + duration;

    // Create and insert event
    EventClass singleEvent(eventTime, nextEventType);
    eventList.insertValue(singleEvent);

    // Output the scheduled event
    cout << "Time: " << currentTime 
         << " Scheduled " << singleEvent << endl;
}

// Handle next event
bool IntersectionSimulationClass::handleNextEvent() {
    EventClass currentEvent;

    // Retrieve the next event
    if (!eventList.removeFront(currentEvent)) {
        cout << "No events to handle. Simulation complete." << endl;
        return false;
    }

    // Advance the simulation time
    currentTime = currentEvent.getTimeOccurs();

    // Check if the event occurs after the simulation end time
    if (currentTime > timeToStopSim) {
        cout << "Next event occurs AFTER the simulation end time "
             << "(" << currentEvent << ")!" << endl;
        return false;
    }
    cout << endl;
    cout << "Handling " << currentEvent << endl;

    // Handle events for when cars arrive
    switch (currentEvent.getType()) {
        case EVENT_ARRIVE_EAST: {
            CarClass carArrival(EAST_DIRECTION, currentTime);
            eastQueue.enqueue(carArrival);
            cout << "Time: " << currentTime << " Car #" << carArrival.getId()
                 << " arrives east-bound - queue length: "
                 << eastQueue.getNumElems() << endl;

            scheduleArrival(EAST_DIRECTION);
            maxEastQueueLength = max(maxEastQueueLength, 
                                     eastQueue.getNumElems());
            break;
        }
        case EVENT_ARRIVE_WEST: {
            CarClass carArrival(WEST_DIRECTION, currentTime);
            westQueue.enqueue(carArrival);
            cout << "Time: " << currentTime << " Car #" << carArrival.getId()
                 << " arrives west-bound - queue length: "
                 << westQueue.getNumElems() << endl;

            scheduleArrival(WEST_DIRECTION);
            maxWestQueueLength = max(maxWestQueueLength, 
                                     westQueue.getNumElems());
            break;
        }
        case EVENT_ARRIVE_NORTH: {
            CarClass carArrival(NORTH_DIRECTION, currentTime);
            northQueue.enqueue(carArrival);
            cout << "Time: " << currentTime << " Car #" << carArrival.getId()
                 << " arrives north-bound - queue length: "
                 << northQueue.getNumElems() << endl;

            scheduleArrival(NORTH_DIRECTION);
            maxNorthQueueLength = max(maxNorthQueueLength, 
                                       northQueue.getNumElems());
            break;
        }
        case EVENT_ARRIVE_SOUTH: {
            CarClass carArrival(SOUTH_DIRECTION, currentTime);
            southQueue.enqueue(carArrival);
            cout << "Time: " << currentTime << " Car #" << carArrival.getId()
                 << " arrives south-bound - queue length: "
                 << southQueue.getNumElems() << endl;

            scheduleArrival(SOUTH_DIRECTION);
            maxSouthQueueLength = max(maxSouthQueueLength, 
                                       southQueue.getNumElems());
            break;
        }
        // Handle events for light change
        case EVENT_CHANGE_YELLOW_EW: {
            cout << "Advancing cars on east-west green" << endl;

            int carsEast = 0, carsWest = 0;
            while (eastQueue.getNumElems() > NO_QUEUE && 
                   carsEast < eastWestGreenTime) {
                CarClass car;
                eastQueue.dequeue(car);
                carsEast++;
                cout << "  Car #" << car.getId() 
                     << " advances east-bound" << endl;
            }
            while (westQueue.getNumElems() > NO_QUEUE && 
                   carsWest < eastWestGreenTime) {
                CarClass car;
                westQueue.dequeue(car);
                carsWest++;
                cout << "  Car #" << car.getId() 
                     << " advances west-bound" << endl;
            }

            cout << "East-bound cars advanced on green: " 
                 << carsEast
                 << " Remaining queue: " << eastQueue.getNumElems() << endl;
            cout << "West-bound cars advanced on green: " 
                 << carsWest
                 << " Remaining queue: " << westQueue.getNumElems() << endl;

            numTotalAdvancedEast += carsEast;
            numTotalAdvancedWest += carsWest;

            currentLight = LIGHT_YELLOW_EW;
            scheduleLightChange();
            break;
        }
        // Handle events for light change
        case EVENT_CHANGE_YELLOW_NS: {
            cout << "Advancing cars on north-south green" << endl;

            int carsNorth = 0, carsSouth = 0;
            while (northQueue.getNumElems() > NO_QUEUE && 
                   carsNorth < northSouthGreenTime) {
                CarClass car;
                northQueue.dequeue(car);
                carsNorth++;
                cout << "  Car #" << car.getId() 
                     << " advances north-bound" << endl;
            }
            while (southQueue.getNumElems() > NO_QUEUE && 
                   carsSouth < northSouthGreenTime) {
                CarClass car;
                southQueue.dequeue(car);
                carsSouth++;
                cout << "  Car #" << car.getId() 
                     << " advances south-bound" << endl;
            }

            cout << "North-bound cars advanced on green: " 
                 << carsNorth
                 << " Remaining queue: " << northQueue.getNumElems() << endl;
            cout << "South-bound cars advanced on green: " 
                 << carsSouth
                 << " Remaining queue: " << southQueue.getNumElems() << endl;

            numTotalAdvancedNorth += carsNorth;
            numTotalAdvancedSouth += carsSouth;

            currentLight = LIGHT_YELLOW_NS;
            scheduleLightChange();
            break;
        }
        // Handle events for light change
        case EVENT_CHANGE_GREEN_NS: {
            cout << "Advancing cars on east-west yellow" << endl;

            int carsEast = 0, carsWest = 0;

            if (eastQueue.getNumElems() == NO_QUEUE) {
                cout << "  No east-bound cars waiting to advance on yellow" 
                     << endl;
            }
            while (eastQueue.getNumElems() > NO_QUEUE && 
                   carsEast < eastWestYellowTime) {
                int yellowAdvanceChance = getUniform(INCLUSIVE_MIN, 
                INCLUSIVE_MAX);
                if (yellowAdvanceChance > percentCarsAdvanceOnYellow && 
                STOP_AT_YELLOW) {
                    cout << "  Next east-bound car will NOT advance on yellow" 
                         << endl;
                    break;
                }
                CarClass car;
                eastQueue.dequeue(car);
                carsEast++;
                cout << "  Car #" << car.getId() << " advances east-bound" 
                     << endl;
            }

            if (westQueue.getNumElems() == NO_QUEUE) {
                cout << "  No west-bound cars waiting to advance on yellow" 
                     << endl;
            }
            while (westQueue.getNumElems() > NO_QUEUE && 
                   carsWest < eastWestYellowTime) {
                int yellowAdvanceChance = getUniform(INCLUSIVE_MIN, 
                INCLUSIVE_MAX);
                if (yellowAdvanceChance > percentCarsAdvanceOnYellow && 
                STOP_AT_YELLOW) {
                    cout << "  Next west-bound car will NOT advance on yellow" 
                         << endl;
                    break;
                }
                CarClass car;
                westQueue.dequeue(car);
                carsWest++;
                cout << "  Car #" << car.getId() << " advances west-bound" 
                     << endl;
            }

            cout << "East-bound cars advanced on yellow: " 
                 << carsEast
                 << " Remaining queue: " << eastQueue.getNumElems() << endl;
            cout << "West-bound cars advanced on yellow: " 
                 << carsWest
                 << " Remaining queue: " << westQueue.getNumElems() << endl;

            numTotalAdvancedEast += carsEast;
            numTotalAdvancedWest += carsWest;

            currentLight = LIGHT_GREEN_NS;
            scheduleLightChange();
            break;
        }
        // Handle events for light change
        case EVENT_CHANGE_GREEN_EW: {
            cout << "Advancing cars on north-south yellow" << endl;

            int carsNorth = 0, carsSouth = 0;

            if (northQueue.getNumElems() == NO_QUEUE) {
                cout << "  No north-bound cars waiting to advance on yellow" 
                     << endl;
            }
            while (northQueue.getNumElems() > NO_QUEUE && 
                   carsNorth < northSouthYellowTime) {
                int yellowAdvanceChance = getUniform(INCLUSIVE_MIN, 
                INCLUSIVE_MAX);
                if (yellowAdvanceChance > percentCarsAdvanceOnYellow && 
                STOP_AT_YELLOW) {
                    cout << "  Next north-bound car will NOT advance on yellow"
                         << endl;
                    break;
                }
                CarClass car;
                northQueue.dequeue(car);
                carsNorth++;
                cout << "  Car #" << car.getId() 
                     << " advances north-bound" << endl;
            }

            if (southQueue.getNumElems() == NO_QUEUE) {
                cout << "  No south-bound cars waiting to advance on yellow" 
                     << endl;
            }
            while (southQueue.getNumElems() > NO_QUEUE && 
                   carsSouth < northSouthYellowTime) {
                int yellowAdvanceChance = getUniform(INCLUSIVE_MIN, 
                INCLUSIVE_MAX);
                if (yellowAdvanceChance > percentCarsAdvanceOnYellow && 
                STOP_AT_YELLOW) {
                    cout << "  Next south-bound car will NOT advance on yellow" 
                         << endl;
                    break;
                }
                CarClass car;
                southQueue.dequeue(car);
                carsSouth++;
                cout << "  Car #" << car.getId() << " advances south-bound" 
                     << endl;
            }

            cout << "North-bound cars advanced on yellow: " 
                 << carsNorth
                 << " Remaining queue: " << northQueue.getNumElems() << endl;
            cout << "South-bound cars advanced on yellow: " 
                 << carsSouth
                 << " Remaining queue: " << southQueue.getNumElems() << endl;

            numTotalAdvancedNorth += carsNorth;
            numTotalAdvancedSouth += carsSouth;

            currentLight = LIGHT_GREEN_EW;
            scheduleLightChange();
            break;
        }
        default:
            cout << "Unknown event type encountered." << endl;
            break;
    }
    return true;
}

void IntersectionSimulationClass::printStatistics(
     ) const
{
  cout << "===== Begin Simulation Statistics =====" << endl;
  cout << "  Longest east-bound queue: " << maxEastQueueLength << endl;
  cout << "  Longest west-bound queue: " << maxWestQueueLength << endl;
  cout << "  Longest north-bound queue: " << maxNorthQueueLength << endl;
  cout << "  Longest south-bound queue: " << maxSouthQueueLength << endl;
  cout << "  Total cars advanced east-bound: " <<
          numTotalAdvancedEast << endl;
  cout << "  Total cars advanced west-bound: " <<
          numTotalAdvancedWest << endl;
  cout << "  Total cars advanced north-bound: " <<
          numTotalAdvancedNorth << endl;
  cout << "  Total cars advanced south-bound: " <<
          numTotalAdvancedSouth << endl;
  cout << "===== End Simulation Statistics =====" << endl;
}
