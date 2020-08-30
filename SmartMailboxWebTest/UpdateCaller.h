/*
 * @file UpdateCaller.h
 * 
 * @author Brian Atwell
 * @description Calls a custom function to be updated at a regular interval.
 */
#include<functional>
 
class UpdateCaller
{
public:

  UpdateCaller(int linterval)
  {
    interval = linterval;
    on_update_handler=NULL;
    lastMS=0;
  }

  void update()
  {
    unsigned long curMS = millis();
    unsigned long diffMS = 0;

    if(curMS > lastMS)
    {
      diffMS = curMS-lastMS;
      if(diffMS >= interval)
      {
        lastMS = curMS;
        callUpdater(diffMS);
      }
    }
    else
    {
      diffMS = 4294967296-lastMS;
      diffMS+=curMS;

      if(diffMS >= interval)
      {
        lastMS = curMS;
        callUpdater(diffMS);
      }
    }
  }

  void SetUpdateHandler(std::function<void (void)> lupdater)
  {
    on_update_handler=lupdater;
  }

protected:
  void callUpdater(unsigned long diffMS)
  {
    if(on_update_handler != NULL)
    {
      on_update_handler();
    }
  }

private:
  unsigned long interval;
  unsigned long lastMS;
  std::function<void (void)> on_update_handler;
  
};

