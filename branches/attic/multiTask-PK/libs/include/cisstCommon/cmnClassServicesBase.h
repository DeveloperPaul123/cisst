/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id$

  Author(s):  Anton Deguet
  Created on: 2004-08-18

  (C) Copyright 2004-2010 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---

*/


/*!
  \file
  \brief Defines the cmnClassServicesBase
*/
#pragma once

#ifndef _cmnClassServicesBase_h
#define _cmnClassServicesBase_h

#include <cisstCommon/cmnForwardDeclarations.h>
#include <cisstCommon/cmnLogLoD.h>

#include <string>
#include <typeinfo>

#include <cisstCommon/cmnExport.h>

/*!
  \brief Base class for class services

  \sa cmnClassServices
*/
class CISST_EXPORT cmnClassServicesBase {
public:
    /*! Type used to define the logging level of detail. */
    typedef cmnLogLoD LogLoDType;

    /*!  Constructor. Sets the name of the class and the Level of Detail
      setting for the class.

      \param className The name to be associated with the class.
      \param typeInfo Runtime type as defined by C++ RTTI
      \param lod The Log Level of Detail setting to be used with this class.
    */
    cmnClassServicesBase(const std::string & className, const std::type_info * typeInfo, LogLoDType lod = CMN_LOG_LOD_RUN_ERROR);


    /*! Virtual destructor.  Does nothing. */
    virtual ~cmnClassServicesBase() {}


    /*! Create a new empty object of the same type as represented by
      this object.  This can be used whenever an object needs to be
      dynamically created (e.g. deserialization, object factory).
      This method uses the C++ "new" operator and the programmers
      should remember to use a matching "delete" if needed.

      \return a pointer to the newly created object or null if object
      cannot be created.  This could happen when the class services
      where created with CMN_NO_DYNAMIC_CREATION.
    */
    virtual cmnGenericObject * Create(void) const = 0;

    /*! Create a new empty array of objects of the same type as
      represented by this object.  This can be used whenever an object
      needs to be dynamically created (e.g. deserialization, object
      factory).  This method uses the C++ "new[size]" operator and the
      programmers should remember to use a matching "delete" if
      needed.

      \return a pointer to the newly created object or null if object
      cannot be created.  This could happen when the class services
      where created with CMN_NO_DYNAMIC_CREATION.
    */
    virtual cmnGenericObject * CreateArray(size_t size) const = 0;

    /*! Create a new empty object of the same type as represented by
      this object using the copy constructor.  This can be used
      whenever an object needs to be dynamically created
      (e.g. deserialization, object factory).  This method uses the
      C++ "new" operator and the programmers should remember to use a
      matching "delete" if needed.

      \param other A object derived from cmnGenericObject which should
      be of the same type as the type represented by this
      cmnClassServices object.

      \return a pointer to the newly created object or null if object
      cannot be created.  This could happen when the class services
      where created with CMN_NO_DYNAMIC_CREATION or if the object
      provided is not of the right type and the copy constructor can
      not be called.
    */
    virtual cmnGenericObject * Create(const cmnGenericObject & other) const = 0;

    /*! Placement new using copy constructor */
    virtual bool Create(cmnGenericObject * existing, const cmnGenericObject & other) const = 0;

    /*! Call destructor explicitely */
    virtual bool Delete(cmnGenericObject * existing) const = 0;

    /*! Get the name associated with the class.

      \return The name of the class as a string.
    */
    inline const std::string & GetName(void) const {
        static std::string NullName("NULL");
        return (NameMember ? (*NameMember) : NullName);
    }

    /*!
      Get the type_info corresponding to the registered class.

      \return Pointer to the class type_info as defined by C++ RTTI.
    */
    inline const std::type_info * TypeInfoPointer(void) const {
        return TypeInfoMember;
    }


    /*! Get the log Level of Detail associated with the class.  This
      is the level used to filter the log messages.

      \return The log Level of Detail.
    */
    inline const LogLoDType & GetLoD(void) const {
        return LoDMember;
    }


    /*! Change the Level of Detail setting for the class.

    \param newLoD The log Level of Detail setting.
    */
    void SetLoD(LogLoDType newLoD);


private:
    /*! The name of the class. */
    const std::string * NameMember;

    const std::type_info * TypeInfoMember;

    /*! The log Level of Detail. */
    LogLoDType LoDMember;
};



/*!
  Class services instantiation.

  This function is templated by the type of the class to be
  registered.  Its implementation is defined by the macro
  #CMN_IMPLEMENT_SERVICES or CMN_IMPLEMENT_SERVICES_TEMPLATED.  In
  both case, the function creates the class services
  (cmnClassServices) and registers it within the class register
  (cmnClassRegister) based on its std::type_info (see typeid and
  C++RTTI for more details) to ensure its uniqueness. */
template <class _class>
cmnClassServicesBase * cmnClassServicesInstantiate(void);


#endif // _cmnClassServicesBase_h
