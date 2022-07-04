#pragma once
#include "SimpleObject.hpp"
#include "Camera.hpp"

namespace FlatFoxWorld{

    class SimpleWorld{
        protected:
        FlatFoxObject::SimpleObject m_Land;

        // This should be more than enough keys for any keyboard
        std::array<int,512> m_Keys;

        //The FlatFoxEngine Will take auto render the objects
        std::vector<std::unique_ptr<FlatFoxObject::SimpleObject> > m_Objects;


        public:

        SimpleWorld();
        ~SimpleWorld();

        inline void SetKeyArray(std::array<int,512> Keys){m_Keys = Keys;}

        inline std::vector<std::unique_ptr<FlatFoxObject::SimpleObject> >* GetWorldObjects(){return &m_Objects;}


    };

}