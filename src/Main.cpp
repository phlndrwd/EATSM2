#include <iostream>

#include "Constants.h"
#include "DataRecorder.h"
#include "Date.h"
#include "Environment.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "Parameters.h"
#include "TimeStep.h"
#include "Timer.h"

int main(int numberOfArguments, char *commandlineArguments[]) {
  std::string parametersFile = "";
  std::string stateFile = "";

  bool showErrorMessage = false;

  switch (numberOfArguments) {
    case 1:
      break;

    case 2: {
      std::string command = commandlineArguments[1];
      if (command == constants::kVersionCommand) {
        std::cout << constants::kSystemName << std::endl;
        std::cout << constants::kSystemVersion << std::endl;
        std::cout << constants::kSystemDate << std::endl;
        std::cout << constants::kSystemTime << std::endl;
        return 0;
      } else
        showErrorMessage = true;

      break;
    }
    case 3: {
      std::string command = commandlineArguments[1];
      std::string filePath = commandlineArguments[2];

      if (command == constants::kParameterFileCommand)
        parametersFile = filePath;
      else if (command == constants::kStateFileCommand)
        stateFile = filePath;
      else if (command == constants::kRestartCommand) {
        parametersFile = filePath + constants::kInputParametersFileName;
        stateFile = filePath + constants::kModelStateFileName;
      } else
        showErrorMessage = true;
      break;
    }
    case 5: {
      std::string firstTerm = commandlineArguments[1];
      std::string secondTerm = commandlineArguments[2];
      std::string thirdTerm = commandlineArguments[3];
      std::string fourthTerm = commandlineArguments[4];

      if (firstTerm == constants::kParameterFileCommand && secondTerm == constants::kStateFileCommand) {
        parametersFile = thirdTerm;
        stateFile = fourthTerm;
      } else if (firstTerm == constants::kParameterFileCommand && thirdTerm == constants::kStateFileCommand) {
        parametersFile = secondTerm;
        stateFile = fourthTerm;
      } else if (firstTerm == constants::kStateFileCommand && secondTerm == constants::kParameterFileCommand) {
        stateFile = thirdTerm;
        parametersFile = fourthTerm;
      } else if (firstTerm == constants::kStateFileCommand && thirdTerm == constants::kParameterFileCommand) {
        stateFile = secondTerm;
        parametersFile = fourthTerm;
      } else
        showErrorMessage = true;
      break;
    }
    default: {
      showErrorMessage = true;
      break;
    }
  }

  if (showErrorMessage == false) {
    std::cout << constants::kSystemName + " " + constants::kSystemVersion + " starting on "
              << Date::getDateAndTimeString() << "..." << std::endl
              << std::endl;
    FileReader fileReader;
    fileReader.readInputFiles(parametersFile, stateFile);
    Timer timer = Timer(true);
    FileWriter fileWriter;  // Created here to initialise output directory

    unsigned runTimeInSeconds = Parameters::Get()->getRunTimeInSeconds();
    double oneTenthOfRunTimeInSeconds = runTimeInSeconds / 10.0;
    double cumulativeTenthsOfRunTime = 0;
    bool isAlive = true;

    Environment environment;
    TimeStep timeStep;

    std::cout << "Model run due to complete on "
              << Date::getDateAndTimeString(constants::kCompleteDateFormat, runTimeInSeconds) << std::endl
              << std::endl;
    std::cout << "Starting main time loop..." << std::endl;
    do {
      // Update before data collection; calculates essential variables for
      // encounter rates.
      environment.update();

      // Data collection
      if (timeStep.doRecordData() == true) {
        DataRecorder::get()->addDataTo("AxisTimeSteps", timeStep.getTimeStep());
        DataRecorder::get()->addDataTo("TimeSampling", timer.split());
        isAlive = environment.recordData();
      }

      // Text output at the completion of each ten percent of the run
      if (timer.elapsed() >= (unsigned)cumulativeTenthsOfRunTime) {
        cumulativeTenthsOfRunTime = cumulativeTenthsOfRunTime + oneTenthOfRunTimeInSeconds;
        std::cout << "t = " << timeStep.getTimeStep() << constants::kDataDelimiterValue
                  << constants::kWhiteSpaceCharacter << timer.remainingString() << " remaining at "
                  << Date::getDateAndTimeString() << "..." << std::endl;
      }

      timeStep.incrementTimeStep();
      // std::cout << "timer.Elapsed( )> " << timer.Elapsed( ) << ",
      // runTimeInSeconds> " << runTimeInSeconds << ", isAlive> " << isAlive <<
      // std::endl;
    } while (timer.elapsed() < runTimeInSeconds && isAlive == true);

    if (timer.elapsed() >= runTimeInSeconds)
      std::cout << "Main time loop complete." << std::endl << std::endl;
    else
      std::cout << "Heterotroph population crashed. Main time loop aborted." << std::endl << std::endl;

    fileWriter.writeOutputData(environment);
    std::cout << "Total run time " << timer.stop() << "s" << std::endl;
  } else if (showErrorMessage == true) {
    std::cout << "ERROR> Command combination not recognised. System exiting..." << std::endl;
    return 1;
  }
  return 0;
}
