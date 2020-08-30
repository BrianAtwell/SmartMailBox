/*
 * @file ConvertToBinary
 * 
 * @author Brian Atwell
 * @description Converts an integer to a binary on/off.
 *  This uses a minium high limit and a minium low limit.
 *  Any value in between is considered noise.
 */
#include<functional>
 
class BinaryConverter
{
public:

  enum BINARYCONVERTENUM {BINARCONVERT_INVALID, BINARYCONVERT_HIGH, BINARYCONVERT_LOW};

  BinaryConverter(int *lAnalogValue, int lMaxThreshold, int lMinThreshold)
  {
    analogValue = lAnalogValue;
    maxThreshold = lMaxThreshold;
    minThreshold = lMinThreshold;
    lastValid = 0;

    on_change_handler=NULL;
  }

  void setChangeHandler(std::function<void (void)> lon_change)
  {
    on_change_handler = lon_change;
  }

  enum BINARYCONVERTENUM getState()
  {
    return getState(analogValue);
  }

  enum BINARYCONVERTENUM getValidState()
  {
    return getState(&lastValid);
  }

  void update()
  {
    if(*analogValue > maxThreshold)
    {
      if(getValidState() != BINARYCONVERT_HIGH)
      {
        handleOnChange();
      }
      lastValid = *analogValue;
    }

    if(*analogValue < minThreshold)
    {
      if(getValidState() != BINARYCONVERT_LOW)
      {
        handleOnChange();
      }
      
      lastValid = *analogValue;
    }
  }

private:
  void handleOnChange()
  {
    if(on_change_handler != NULL)
    {
      on_change_handler();
    }
  }

  enum BINARYCONVERTENUM getState(int *lAnalogValue)
  {
    if(*lAnalogValue > maxThreshold)
    {
      return BINARYCONVERT_HIGH;
    }
    else if(*lAnalogValue < minThreshold)
    {
      return BINARYCONVERT_LOW;
    }
    
    return BINARCONVERT_INVALID;
  }


private:
  int* analogValue;
  int maxThreshold;
  int minThreshold;
  int lastValid;

  std::function<void (void)> on_change_handler;
};

