#include <iostream>

#include "Constants.h"
#include "DataRecorder.h"
#include "Date.h"
#include "Environment.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "HeterotrophProcessor.h"
#include "Heterotrophs.h"
#include "Parameters.h"
#include "Strings.h"
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
      if (command == Constants::cVersionCommand) {
        std::cout << Constants::cSystemName << std::endl;
        std::cout << Constants::cSystemVersion << std::endl;
        std::cout << Constants::cSystemDate << std::endl;
        std::cout << Constants::cSystemTime << std::endl;
        return 0;
      } else
        showErrorMessage = true;

      break;
    }
    case 3: {
      std::string command = commandlineArguments[1];
      std::string filePath = commandlineArguments[2];

      if (command == Constants::cParameterFileCommand)
        parametersFile = filePath;
      else if (command == Constants::cStateFileCommand)
        stateFile = filePath;
      else if (command == Constants::cRestartCommand) {
        parametersFile = filePath + Constants::cInputParametersFileName;
        stateFile = filePath + Constants::cModelStateFileName;
      } else
        showErrorMessage = true;
      break;
    }
    case 5: {
      std::string firstTerm = commandlineArguments[1];
      std::string secondTerm = commandlineArguments[2];
      std::string thirdTerm = commandlineArguments[3];
      std::string fourthTerm = commandlineArguments[4];

      if (firstTerm == Constants::cParameterFileCommand && secondTerm == Constants::cStateFileCommand) {
        parametersFile = thirdTerm;
        stateFile = fourthTerm;
      } else if (firstTerm == Constants::cParameterFileCommand && thirdTerm == Constants::cStateFileCommand) {
        parametersFile = secondTerm;
        stateFile = fourthTerm;
      } else if (firstTerm == Constants::cStateFileCommand && secondTerm == Constants::cParameterFileCommand) {
        stateFile = thirdTerm;
        parametersFile = fourthTerm;
      } else if (firstTerm == Constants::cStateFileCommand && thirdTerm == Constants::cParameterFileCommand) {
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
    std::cout << Constants::cSystemName + " " + Constants::cSystemVersion + " starting on "
              << Date::GetDateAndTimeString() << "..." << std::endl
              << std::endl;
    FileReader fileReader;
    fileReader.ReadInputFiles(parametersFile, stateFile);
    Timer timer = Timer(true);
    FileWriter fileWriter;  // Created here to initialise output directory

    unsigned runTimeInSeconds = Parameters::Get()->GetRunTimeInSeconds();
    double oneTenthOfRunTimeInSeconds = runTimeInSeconds / 10.0;
    double cumulativeTenthsOfRunTime = 0;
    bool isAlive = true;

    Environment environment;
    TimeStep timeStep;

    std::cout << "Model run due to complete on "
              << Date::GetDateAndTimeString(Constants::cCompleteDateFormat, runTimeInSeconds) << std::endl
              << std::endl;
    std::cout << "Starting main time loop..." << std::endl;
    do {
      // Update before data collection; calculates essential variables for
      // encounter rates.
      environment.Update();

      // Data collection
      if (timeStep.DoRecordData() == true) {
        DataRecorder::Get()->AddDataTo("AxisTimeSteps", timeStep.GetTimeStep());
        DataRecorder::Get()->AddDataTo("TimeSampling", timer.Split());
        isAlive = environment.RecordData();
      }

      // Text output at the completion of each ten percent of the run
      if (timer.Elapsed() >= (unsigned)cumulativeTenthsOfRunTime) {
        cumulativeTenthsOfRunTime = cumulativeTenthsOfRunTime + oneTenthOfRunTimeInSeconds;
        std::cout << "t = " << timeStep.GetTimeStep() << Constants::cDataDelimiterValue
                  << Constants::cWhiteSpaceCharacter << timer.RemainingString() << " remaining at "
                  << Date::GetDateAndTimeString() << "..." << std::endl;
      }

      timeStep.IncrementTimeStep();
      // std::cout << "timer.Elapsed( )> " << timer.Elapsed( ) << ",
      // runTimeInSeconds> " << runTimeInSeconds << ", isAlive> " << isAlive <<
      // std::endl;
    } while (timer.Elapsed() < runTimeInSeconds && isAlive == true);

    if (timer.Elapsed() >= runTimeInSeconds)
      std::cout << "Main time loop complete." << std::endl << std::endl;
    else
      std::cout << "Heterotroph population crashed. Main time loop aborted." << std::endl << std::endl;

    fileWriter.WriteOutputData(environment);
    std::cout << "Total run time " << timer.Stop() << "s" << std::endl;
  } else if (showErrorMessage == true) {
    std::cout << "ERROR> Command combination not recognised. System exiting..." << std::endl;
    return 1;
  }
  return 0;
}
