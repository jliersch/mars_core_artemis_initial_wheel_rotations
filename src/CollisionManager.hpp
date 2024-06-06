/**
 * \file CollisionMangaer.hpp
 * \author Malte Langosz
 * \brief
 *
 */

#pragma once

#include <memory>
#include <vector>

#include <mars_interfaces/sim/ControlCenter.h>
#include <mars_interfaces/sim/CollisionInterface.hpp>
#include <mars_interfaces/sim/CollisionHandler.hpp>
#include <mars_interfaces/sim/ContactPluginInterface.hpp>

#include <envire_core/items/Item.hpp>
#include <envire_core/events/GraphItemEventDispatcher.hpp>

namespace mars
{
    namespace core
    {
        class CollisionManager : public envire::core::GraphItemEventDispatcher<envire::core::Item<interfaces::ContactPluginInterfaceItem>>
        {
        public:
            CollisionManager(const std::shared_ptr<interfaces::ControlCenter>& controlCenter);
            virtual ~CollisionManager();

            void addCollisionHandler(const std::string &name1, const std::string &name2,
                                     std::shared_ptr<interfaces::CollisionHandler> collisionHandler);
            void handleContacts();
            void addCollisionInterfaceItem(const interfaces::CollisionInterfaceItem &item);
            std::vector<interfaces::ContactData>& getContactVector();
            void updateTransforms();

        protected:
            virtual void itemAdded(const envire::core::TypedItemAddedEvent<envire::core::Item<interfaces::ContactPluginInterfaceItem>>& event) override;
            virtual void itemRemoved(const envire::core::TypedItemRemovedEvent<envire::core::Item<interfaces::ContactPluginInterfaceItem>>& event) override;

        private:
            void setupContactVector();
            void applyContactPlugins();

            std::map<std::pair<std::string, std::string>, std::shared_ptr<interfaces::CollisionHandler>> collisionHandlers;
            std::vector<interfaces::ContactData> contactVector;
            std::vector<interfaces::CollisionInterfaceItem> collisionItems;
            std::vector<interfaces::ContactPluginInterfaceItem> contactPluginItems;
        };
    } // end of namespace core
} // end of namespace mars
