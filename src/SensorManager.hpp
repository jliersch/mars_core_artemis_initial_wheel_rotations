/**
 * \file SensorManager.hpp
 * \author  Malte Langosz
 * \brief SensorManager implements SensorManagerInterface and
 * manages all sensors and all sensor
 * operations that are used for the communication between the simulation
 * modules.
 *
 */

#pragma once

#include <mars_interfaces/sim/SensorManagerInterface.h>
#include <mars_interfaces/sim/ControlCenter.h>
#include <mars_utils/Mutex.h>
#include <configmaps/ConfigData.h>

namespace mars
{
    namespace core
    {
        class SensorReloadHelper
        {
        public:
            SensorReloadHelper(std::string type, interfaces::BaseConfig *config):
                type(type), config(config) { }

            std::string type;
            interfaces::BaseConfig *config;
        };


        /**
         * \brief "SensorManager" imlements the interfaces for all sensor
         * operations that are used for the communication between the simulation
         * modules.
         *
         * \warning It is very important to assure the serialization between the threads to
         * have the desired results. Currently the verified use of the functions
         * is only guaranteed by calling it within the main thread (update
         * callback from \c gui_thread).
         */
        class SensorManager : public interfaces::SensorManagerInterface
        {
        public:

            /**
             * \brief Constructor.
             *
             * \param c The pointer to the ControlCenter of the simulation.
             */
            SensorManager(interfaces::ControlCenter *c);

            /**
             * \brief Destructor.
             */
            virtual ~SensorManager() override {}

            /**
             * \brief Add a sensor to the simulation.
             *
             * \param sensorS A pointer to the BaseSensor that defines the new sensor.
             *
             * \param reload Used internally by the simulation. The
             * default value is \c false. If this param is set to \c true the new sensor
             * will not be reloaded after a reset of the simulation.
             *
             * \return The unique id of the newly added sensor.
             */
            //virtual unsigned long addSensor(BaseSensor *sensorS, bool reload = false);

            /**
             *\brief Returns true, if the sensor with the given id exists.
             *
             * \param id The id of the sensor to look for.
             * \return Whether the node exists.
             */
            virtual bool exists(unsigned long index) const override;

            /**
             * \brief Gives information about core exchange data for sensors.
             *
             * \param sensorList A pointer to a vector that is filled with a
             * core_objects_exchange struct for every sensor. The vector is cleared
             * in the beginning of this function.
             */
            virtual void getListSensors(std::vector<interfaces::core_objects_exchange> *sensorList) const override;

            /**
             * \brief Gives information about core exchange data for camera sensors.
             *
             * \param cameraList A pointer to a vector that is filled with a
             * core_objects_exchange struct for every camera sensor. The vector is cleared
             * in the beginning of this function.
             */
            //  virtual void getListCameraSensors(std::vector<core_objects_exchange> *cameraList) const;
            //  Depricated could be done by the user

            /**
             * \brief Gives all information of a certain sensor.
             *
             * \param index The unique id of the sensor to get information for.
             *
             * \return A pointer to the BaseSensor of the sensor with the given id.
             */
            virtual const interfaces::BaseSensor* getFullSensor(unsigned long index) const override;
            unsigned long getSensorID(std::string name) const override;

            /**
             * \brief Removes a sensor from the simulation.
             *
             * \param index The unique id of the sensor to remove form the simulation.
             */
            virtual void removeSensor(unsigned long index) override;

            /**
             * \brief This function returns the SimSensor object for a given index.
             *
             * \param name The index of the sensor to get the core sensor object.
             *
             * \returns Returns a pointer to the corresponding sensor object.
             */
            virtual interfaces::BaseSensor* getSimSensor(unsigned long index) const override;
#if 0

            /**
             * \brief This function returns the GridSensor object for a given index.
             *
             * \param name The index of the sensor to get the core sensor object.
             *
             * \returns Returns a pointer to the corresponding sensor object.
             */
            virtual interfaces::GridSensorInterface* getRayGridSensor(unsigned long index) const;

            /**
             * \brief This function provides the CameraSensor object for a given index.
             *
             * \param cs The cameraStruct of the sensor.
             *
             * \param index The index of the sensor to get the \c cameraStruct
             * \returns Returns 1 if the sensor is found, 0 otherwise.
             */
            virtual int getCameraSensor(interfaces::cameraStruct *cs, int index) const;
#endif

            /**
             * \brief This function provides the sensor data for a given index.
             *
             * \param data The sensor data of the sensor.
             *
             * \param index The index of the sensor to get the data
             */
            virtual int getSensorData(unsigned long id, interfaces::sReal **data) const override;

            /**
             *\brief Returns the number of sensors that are currently present in the simulation.
             *
             *\return The number of all sensors.
             */
            virtual int getSensorCount(void) const override;

            /**
             * \brief Destroys all sensors in the simulation.
             *
             * \details The \c clear_all flag indicates if the reload sensors should
             * be destroyed as well. If set to \c false they are left intact.
             *
             * \param clear_all Indicates if the reload sensors should
             * be destroyed as well. If set to \c false they are left intact.
             */
            virtual void clearAllSensors(bool clear_all = false) override;

            /**
             * \brief This function reloads all sensors from a temporary sensor pool.
             *
             * \details All sensors that have been added with \c reload value as \c true
             * are added back to the simulation again with a \c reload value of \c true.
             */
            virtual void reloadSensors(void) override;

            //virtual void addSensorType(const std::string &name,  BaseSensor* (*func)(interfaces::ControlCenter*,const unsigned long int,const std::string,QDomElement*));
            //void addSensorType(const std::string &name, BaseSensor* (*func)(interfaces::ControlCenter*,const unsigned long int, const std::string, mars::ConfigMap*));
            void addSensorType(const std::string &name, interfaces::BaseSensor* (*func)(interfaces::ControlCenter*, interfaces::BaseConfig*));

            //void addQdomParser(const std::string, BaseConfig* (*)(QDomElement*));
            void addMarsParser(const std::string, interfaces::BaseConfig* (*)(interfaces::ControlCenter*, configmaps::ConfigMap*));

            //virtual BaseSensor* createAndAddSensor(const std::string &type_name, std::string name="",QDomElement* config=0, bool reload=true);
            virtual unsigned long createAndAddSensor(configmaps::ConfigMap* config, bool reload=true) override;
            virtual unsigned long createAndAddSensor(const std::string &type_name,interfaces::BaseConfig *config, bool reload=false) override;

        private:
            // Setup
            void addSensorTypes();
            void addMarsParsers();

            //! a pointer to the control center
            interfaces::ControlCenter *control;

            //! a containter for all sensors currently present in the simulation
            std::map<unsigned long, interfaces::BaseSensor*> simSensors;
            std::unique_ptr<interfaces::IDManager> idManager_;

            //! a mutex fot the sensor containter
            mutable utils::Mutex simSensorsMutex;

            //std::map<const std::string,BaseSensor* (*)(interfaces::ControlCenter*,const unsigned long int,const std::string,QDomElement*)> availibleSensors;
            //std::map<const std::string,BaseSensor* (*)(interfaces::ControlCenter*,const unsigned long int, const std::string, mars::ConfigMap*)> availableSensors2;
            std::map<const std::string, interfaces::BaseSensor* (*)(interfaces::ControlCenter*, interfaces::BaseConfig*)> availableSensors;

            //std::map<const std::string,BaseConfig* (*)(QDomElement*)> qDomParser;
            std::map<const std::string, interfaces::BaseConfig* (*)(interfaces::ControlCenter*, configmaps::ConfigMap*)> marsParser;

        }; // class SensorManager

    } // end of namespace core
} // end of namespace mars

