/*
 * File: SensorAnalog.h
 * 
 * Summary: This file handles analog "sensor" or datatype.
 */

namespace SmartMailBox
{
  namespace Sensors
  {

    template<typename DataType>
    class SensorAbstract
    {
    public:
      virtual int isChanged() = 0;
      
      virtual void update() = 0;

      virtual DataType getData() = 0;
    }

    template<typename DataType>
    class SensorAnalog : public SensorAbstract<DataType>
    {
    public:
      SensorAnalog(DataType *lpData)
      {
        ptrData = lpData;
        isUpdated=false;
      }
    
      int isChanged()
      {
        if(!isUpdated)
        {
          return -1;
        }
        return prevData = *ptrData;
      }

      void update()
      {
        prevData = *ptrData;
        
        isUpdated=true;
      }

      DataType getData()
      {
        return *ptrData;
      }
    private:

      bool isUpdated;
      DataType prevData;
      DataType* ptrData;
    }

    class SensorDigital : public SensorAbstract<bool>
    {
    public:
      SensorDigital(bool *lpData)
      {
        ptrData = lpData;
        isUpdated=false;
      }
    
      int isChanged()
      {
        if(!isUpdated)
        {
          return -1;
        }
        return prevData = *ptrData;
      }

      void update()
      {
        prevData = *ptrData;
        
        isUpdated=true;
      }

      bool getData()
      {
        return *ptrData;
      }
    private:

      bool isUpdated;
      bool prevData;
      bool* ptrData;
    }
  }
}

