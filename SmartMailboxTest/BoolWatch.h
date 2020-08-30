/*
 * @file BoolWatch.h
 * 
 * @author Brian Atwell
 * @description Watches a bool value for changes and calls a function.
 */

#include<functional>

 class BoolWatch
{
public:

  BoolWatch(bool *lboolValue)
  {
    boolValue = lboolValue;
    lastValue=*boolValue;

    on_change_handler=NULL;
  }

  void setChangeHandler(std::function<void (void)> lon_change)
  {
    on_change_handler = lon_change;
  }

  void update()
  {
    if((*boolValue & 1) != (lastValue & 1))
    {
      handleOnChange();
      lastValue = *boolValue;
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


private:
  bool* boolValue;
  bool lastValue;

  std::function<void (void)> on_change_handler;
};

