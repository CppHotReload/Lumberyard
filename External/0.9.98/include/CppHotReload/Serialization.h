//
//  Copyright (c) 2014 - 2019 C++ Hot Reload SL. All rights reserved
//
//  This file is subject to the terms and conditions defined in
//  file 'LICENSE.txt', which is part of this source code package.
//

#pragma once
//
// C/C++
//
#include <limits>
#include <iostream>
#include <sstream>
#include <string>
//
// Containers
//
#include <unordered_map>
#include <set>
#include <vector>

#include <set>
#include <vector>
#include <deque>
#include <forward_list>
#include <list>
#include <array>
#include <unordered_map>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <queue>

namespace CppHotReload
{
    using Char						= char;
	using OStringStream				= std::ostringstream;
	using StringStream				= std::stringstream;
	using InputFileStream			= std::ifstream;
	using OutputFileStream			= std::ofstream;
    using InputFileStreamIterator	= std::istreambuf_iterator<char>;

	class Serialization
	{
	public:
		static void*  VOID_PTR;
		static void*& EMPTY_PTR_REF;

		static const std::string keyword_namespace;
		static const std::string keyword_class;
		static const std::string keyword_struct;

		static const std::string keyword_voidPtr;
		static const std::string keyword_bool;
		static const std::string keyword_char;
        static const std::string keyword_char16_t;
        static const std::string keyword_char32_t;
        static const std::string keyword_wchar_t;
		static const std::string keyword_schar;
        static const std::string keyword_short;
		static const std::string keyword_short_int;
		static const std::string keyword_int;
		static const std::string keyword_long;
		static const std::string keyword_long_int;
		static const std::string keyword_long_long;
		static const std::string keyword_long_long_int;
		static const std::string keyword_float;
		static const std::string keyword_double;
		static const std::string keyword_long_double;
		static const std::string keyword_signed;
		static const std::string keyword_unsigned;

		static const std::string qualifier_const;
		static const std::string qualifier_static;
		static const std::string qualifier_volatile;
		static const std::string qualifier_mutable;
		static const std::string qualifier_typedef;
        static const std::string qualifier_using;

		static const std::string operator_none;
		static const std::string operator_reference;
		static const std::string operator_deference;

        static const std::string container_set;
        static const std::string container_vector;
        static const std::string container_deque;
        static const std::string container_forward_list;
        static const std::string container_list;
        static const std::string container_array;
        static const std::string container_map;
        static const std::string container_multiset;
        static const std::string container_multimap;
        static const std::string container_unordered_map;
        static const std::string container_unordered_set;
        static const std::string container_unordered_multiset;
        static const std::string container_unordered_multimap;
        static const std::string container_stack;
        static const std::string container_queue;
        static const std::string container_priority_queue;
	};
	
	enum QualifierType
	{
		NoneQualifier = 0,
		Static,
		Volatile,
		Mutable,
        TemplateParam,  // used to store container params
        Array,          // special treatment to copy
	};

	enum SignType
	{
		NoneSign = 0,
		Signed,
		Unsigned,
	};

	enum OperatorType
	{
		NoneOperator = 0,
		Deference,
		Reference,
	};

	enum TypeName
	{
		Class = 0,
		Struct,
		Namespace,
	};

	struct ContentParsed
	{
		std::string name;
		std::string content;
	};
    //
	// Variable
    //
	template<class T>
	class Variable
	{
    private:
        bool hasChangeToApply;
		QualifierType qualifier;
		SignType      sign;
		OperatorType  operatorType;

		T			  value;
        size_t        length;
		std::string   typeName;
		std::string   varName;

    public:
        //
        // Properties
        //
        QualifierType GetQualifier()    const { return qualifier;    }
        SignType      GetSign()         const { return sign;         }
        OperatorType  GetOperatorType() const { return operatorType; }

        T			  GetValue()        const { return value;        }
        std::string   GetTypeName()     const { return typeName;     }
        std::string   GetName()         const { return varName;      }
        size_t        GetLength()       const { return length;       }

        void          SetValue    (T newValue){ value = newValue; hasChangeToApply = true; }
        T             ExtractValue()          { hasChangeToApply = false; return value;    }
        void          SetTypeName(const std::string& newTypeName) { typeName = newTypeName; }

        bool          HasToUpdate() const     { return hasChangeToApply; }
		
        Variable(const std::string& _typeName, const std::string& _varName, T _value,
                 QualifierType _qualifier, SignType _sign, OperatorType _operatorType, size_t _length = 1)
        : hasChangeToApply(false)
		, qualifier(_qualifier)
		, sign(_sign)
		, operatorType(_operatorType)
		, value(_value)
		, length(_length)
        , typeName(_typeName)
        , varName(_varName)
        {

        }
        //
        // VarManager and Variable are temporal data holders or transport data temporally.
        // In the case of custom types, as void* you may not like delete that memory as is not
        // copied for performance reasons. If you desire so, call this method
        //
        void Release(void)
        {
            std::free(value);
        }
		//
		// Note: the var type name printed will be the original one. "long long" is the same as
		// "long long int"
        //
        std::string ToString(void);
	};
    //
    // Way to send by network in binary format a Var Manager
    //
    struct VarManagerNetworkHeader
    {
        size_t size;
        size_t bools_size;
        size_t floats_size;
        size_t doubles_size;
        size_t long_doubles_size;
        size_t chars_size;
        size_t chars16s_size;
        size_t chars32s_size;
        size_t wchars_size;
        size_t schars_size;
        size_t shorts_size;
        size_t ints_size;
        size_t longs_size;
        size_t long_longs_size;
        size_t uchars_size;
        size_t ushorts_size;
        size_t uints_size;
        size_t ulongs_size;
        size_t ulong_longs_size;
        size_t customTypes_size;
    };
    //
    // VarManager
    //
	struct VarManager
	{
		std::string  fileName;
		std::string  name;
        std::string  moduleName;
		std::string  typeName;
		std::string  keyName;
		std::string  uuid;

		unsigned int checksum;
		unsigned int version;

        bool deleteCustomTypesAndBakedData;
        bool hasAllowFullExtraction;
		bool isTemplate;
        //
		// Fundamental types
        //
        std::unordered_map< std::string, Variable<bool>*               > bools;
        std::unordered_map< std::string, Variable<float>*              > floats;
        std::unordered_map< std::string, Variable<double>*             > doubles;
        std::unordered_map< std::string, Variable<long double>*        > long_doubles;

        std::unordered_map< std::string, Variable<char>*               > chars;
        std::unordered_map< std::string, Variable<char16_t>*           > chars16;
        std::unordered_map< std::string, Variable<char32_t>*           > chars32;
        std::unordered_map< std::string, Variable<wchar_t>*            > wchars;
        std::unordered_map< std::string, Variable<signed   char>*      > schars;
        std::unordered_map< std::string, Variable<signed   short>*     > shorts;
        std::unordered_map< std::string, Variable<signed   int>*       > ints;
        std::unordered_map< std::string, Variable<signed   long>*      > longs;
        std::unordered_map< std::string, Variable<signed   long long>* > long_longs;

        std::unordered_map< std::string, Variable<unsigned char>*  	   > uchars;
        std::unordered_map< std::string, Variable<unsigned short>*     > ushorts;
        std::unordered_map< std::string, Variable<unsigned int>*  	   > uints;
        std::unordered_map< std::string, Variable<unsigned long>*      > ulongs;
        std::unordered_map< std::string, Variable<unsigned long long>* > ulong_longs;
        //
        // Containers
        //
        std::unordered_map< std::string, Variable<std::string>* > containers;
        //
        // Custom types
        //
		std::unordered_map< std::string, Variable<void*>* > customTypes;

		std::set< std::string > fileDependencies;
		std::vector< std::string > baseTypes;

		std::vector< std::string > methods;

		VarManager* parent;

		std::vector<VarManager*> varManagers;

        explicit VarManager(VarManager* parent,
                            const std::string& fileName, const std::string& name, const std::string& moduleName,
                            const std::string& typeName, const std::string& keyName);
        //
        // Meant for Network Communication
        // hasAllowFullExtraction = true;
        // deleteCustomTypesAndBakedData = true;
        //
        explicit VarManager(const std::string& name);
        ~VarManager(void);
        
        static VarManager* FromBinaryData(char* bakedBinaryData, size_t bakedBinaryDataLength);
        static VarManager* FromBinaryData(std::istream& in);
        static VarManager* FromBinaryData(const VarManagerNetworkHeader& vmHeader, char* dataBody);
        static char* ToBinaryData(VarManager* vm, size_t& length);
        //
		// Fast & basic deploy
        //
		std::string ToString(size_t indentCount = 0) const;
		std::string ToXmlString(size_t indentCount = 0) const;
		std::string ToEncodedXmlString(size_t indentCount = 0) const;
        //
        // Generates the content into a char*
        //
        void   BakeBinaryData(void);
        void   ReleaseBinaryData(void);
        char*  bakedBinaryData;
        size_t bakedBinaryDataLength;
        //
        // Getters
        //
        size_t size() const;

        bool                GetBool             (const std::string& varName, bool               defaultValue =   false) const;
        float               GetFloat            (const std::string& varName, float              defaultValue =    0.0f) const;
        double              GetDouble           (const std::string& varName, double             defaultValue =     0.0) const;
        long double         GetLongDouble       (const std::string& varName, long double        defaultValue =     0.0) const;
        char                GetChar             (const std::string& varName, char               defaultValue =     'c') const;
        char16_t            GetChar16           (const std::string& varName, char16_t           defaultValue =     'c') const;
        char32_t            GetChar32           (const std::string& varName, char32_t           defaultValue =     'c') const;
        wchar_t             GetWChar            (const std::string& varName, wchar_t            defaultValue =     'c') const;
        signed char         GetSignedChar       (const std::string& varName, signed char        defaultValue =     'c') const;
        signed short        GetShort            (const std::string& varName, signed short       defaultValue =       0) const;
        signed int          GetInt              (const std::string& varName, signed int         defaultValue =       0) const;
        signed long         GetLong             (const std::string& varName, signed long        defaultValue =       0) const;
        signed long long    GetLongLong         (const std::string& varName, signed long long   defaultValue =       0) const;
        unsigned char       GetUnsignedChar     (const std::string& varName, unsigned char      defaultValue =     'c') const;
        unsigned short      GetUnsignedShort    (const std::string& varName, unsigned short     defaultValue =       0) const;
        unsigned int        GetUnsignedInt      (const std::string& varName, unsigned int       defaultValue =       0) const;
        unsigned long       GetUnsignedLong     (const std::string& varName, unsigned long      defaultValue =       0) const;
        unsigned long long  GetUnsignedLongLong (const std::string& varName, unsigned long long defaultValue =       0) const;
        void*               GetCustomType       (const std::string& varName, void*              defaultValue = nullptr) const;
        //
        // Extraction of the values
        // This marks as modified internally the values
        //
        bool                ExtractBool             (const std::string& varName, bool               defaultValue =   false) const;
        float               ExtractFloat            (const std::string& varName, float              defaultValue =    0.0f) const;
        double              ExtractDouble           (const std::string& varName, double             defaultValue =     0.0) const;
        long double         ExtractLongDouble       (const std::string& varName, long double        defaultValue =     0.0) const;
        char                ExtractChar             (const std::string& varName, char               defaultValue =     'c') const;
        char16_t            ExtractChar16           (const std::string& varName, char16_t           defaultValue =     'c') const;
        char32_t            ExtractChar32           (const std::string& varName, char32_t           defaultValue =     'c') const;
        wchar_t             ExtractWChar            (const std::string& varName, wchar_t            defaultValue =     'c') const;
        signed char         ExtractSignedChar       (const std::string& varName, signed char        defaultValue =     'c') const;
        signed short        ExtractShort            (const std::string& varName, signed short       defaultValue =       0) const;
        signed int          ExtractInt              (const std::string& varName, signed int         defaultValue =       0) const;
        signed long         ExtractLong             (const std::string& varName, signed long        defaultValue =       0) const;
        signed long long    ExtractLongLong         (const std::string& varName, signed long long   defaultValue =       0) const;
        unsigned char       ExtractUnsignedChar     (const std::string& varName, unsigned char      defaultValue =     '0') const;
        unsigned short      ExtractUnsignedShort    (const std::string& varName, unsigned short     defaultValue =       0) const;
        unsigned int        ExtractUnsignedInt      (const std::string& varName, unsigned int       defaultValue =       0) const;
        unsigned long       ExtractUnsignedLong     (const std::string& varName, unsigned long      defaultValue =       0) const;
        unsigned long long  ExtractUnsignedLongLong (const std::string& varName, unsigned long long defaultValue =       0) const;
        void*               ExtractCustomType       (const std::string& varName, void*              defaultValue = nullptr) const;

        std::string GetTypeName(const std::string& varName) const
        {
            for (VarManager* subVarManager : varManagers)
            {
                if (varName == subVarManager->name)
                {
                    return subVarManager->keyName;
                }
            }

            return varName;
        }

        std::string GetTypeName(void) const
        {
            return typeName;
        }
        //
        // Note: t is the type name, if is not set will use the same as value. This applies only to void*
        // by default will assign for t value "void*" which is not meaningful. In that case assign a meaningful value to t
        //
        template<typename T>
        void AddVariable(const std::string& varName, T value,
                         QualifierType q = NoneQualifier, SignType s = NoneSign, OperatorType o = NoneOperator)
        { printf("[VarManager] AddVariable called with a non-implemented type for %s\n", varName.c_str()); }
		//
		// In this case is lost the type name meaning, which for information is important
		//
		template<typename T = void*>
		inline void AddVariable(const std::string& varName, void* value, QualifierType q, SignType s, OperatorType o, const std::string& t)
		{
			customTypes[varName] = new Variable<void*>(t.empty() ? "void*" : t, varName, value, q, s, o);
		}
        //
        // If the variable is not found the detault value T will be returned
        //
        template<typename T>
        T ExtractValue(const std::string& varName, T defaultValue = 0) const
        { printf("[VarManager] ExtractValue called with a non-implemented type for %s, returning default value\n", varName.c_str()); return defaultValue; }
        //
        // Arrays
        //
        template<typename T>
        void AddVariable(const std::string& varName, T value, size_t length)
        { printf("[VarManager] AddVariable called with a non-implemented type for %s\n", varName.c_str()); }
    };
    //
    // AddVariable Specializations
    //
    template<> void VarManager::AddVariable(const std::string& varName, bool                value, QualifierType q, SignType s, OperatorType o);
    template<> void VarManager::AddVariable(const std::string& varName, float               value, QualifierType q, SignType s, OperatorType o);
    template<> void VarManager::AddVariable(const std::string& varName, double              value, QualifierType q, SignType s, OperatorType o);
    template<> void VarManager::AddVariable(const std::string& varName, long double         value, QualifierType q, SignType s, OperatorType o);
    template<> void VarManager::AddVariable(const std::string& varName, char                value, QualifierType q, SignType s, OperatorType o);
    template<> void VarManager::AddVariable(const std::string& varName, char16_t            value, QualifierType q, SignType s, OperatorType o);
    template<> void VarManager::AddVariable(const std::string& varName, char32_t            value, QualifierType q, SignType s, OperatorType o);
    template<> void VarManager::AddVariable(const std::string& varName, wchar_t             value, QualifierType q, SignType s, OperatorType o);
    template<> void VarManager::AddVariable(const std::string& varName, signed char         value, QualifierType q, SignType s, OperatorType o);
    template<> void VarManager::AddVariable(const std::string& varName, short               value, QualifierType q, SignType s, OperatorType o);
    template<> void VarManager::AddVariable(const std::string& varName, int                 value, QualifierType q, SignType s, OperatorType o);
    template<> void VarManager::AddVariable(const std::string& varName, long                value, QualifierType q, SignType s, OperatorType o);
    template<> void VarManager::AddVariable(const std::string& varName, long long           value, QualifierType q, SignType s, OperatorType o);
    template<> void VarManager::AddVariable(const std::string& varName, unsigned char       value, QualifierType q, SignType s, OperatorType o);
    template<> void VarManager::AddVariable(const std::string& varName, unsigned short      value, QualifierType q, SignType s, OperatorType o);
    template<> void VarManager::AddVariable(const std::string& varName, unsigned int        value, QualifierType q, SignType s, OperatorType o);
    template<> void VarManager::AddVariable(const std::string& varName, unsigned long       value, QualifierType q, SignType s, OperatorType o);
    template<> void VarManager::AddVariable(const std::string& varName, unsigned long long  value, QualifierType q, SignType s, OperatorType o);
    template<> void VarManager::AddVariable(const std::string& varName, void*               value, QualifierType q, SignType s, OperatorType o);
    //
    // Arrays
    //
    template<> void VarManager::AddVariable(const std::string& varName, bool*               value, size_t length);
    template<> void VarManager::AddVariable(const std::string& varName, float*              value, size_t length);
    template<> void VarManager::AddVariable(const std::string& varName, double*             value, size_t length);
    template<> void VarManager::AddVariable(const std::string& varName, long double*        value, size_t length);
    template<> void VarManager::AddVariable(const std::string& varName, char*               value, size_t length);
    template<> void VarManager::AddVariable(const std::string& varName, char16_t*           value, size_t length);
    template<> void VarManager::AddVariable(const std::string& varName, char32_t*           value, size_t length);
    template<> void VarManager::AddVariable(const std::string& varName, wchar_t*            value, size_t length);
    template<> void VarManager::AddVariable(const std::string& varName, signed char*        value, size_t length);
    template<> void VarManager::AddVariable(const std::string& varName, short*              value, size_t length);
    template<> void VarManager::AddVariable(const std::string& varName, int*                value, size_t length);
    template<> void VarManager::AddVariable(const std::string& varName, long*               value, size_t length);
    template<> void VarManager::AddVariable(const std::string& varName, long long*          value, size_t length);
    template<> void VarManager::AddVariable(const std::string& varName, unsigned char*      value, size_t length);
    template<> void VarManager::AddVariable(const std::string& varName, unsigned short*     value, size_t length);
    template<> void VarManager::AddVariable(const std::string& varName, unsigned int*       value, size_t length);
    template<> void VarManager::AddVariable(const std::string& varName, unsigned long*      value, size_t length);
    template<> void VarManager::AddVariable(const std::string& varName, unsigned long long* value, size_t length);
    template<> void VarManager::AddVariable(const std::string& varName, void*               value, size_t length);
    //
    // ExtractValue Specializations
    //
    template<> bool                 VarManager::ExtractValue(const std::string& varName, bool               defaultValue) const;
    template<> float                VarManager::ExtractValue(const std::string& varName, float              defaultValue) const;
    template<> double               VarManager::ExtractValue(const std::string& varName, double             defaultValue) const;
    template<> long double          VarManager::ExtractValue(const std::string& varName, long double        defaultValue) const;
    template<> char                 VarManager::ExtractValue(const std::string& varName, char               defaultValue) const;
    template<> char16_t             VarManager::ExtractValue(const std::string& varName, char16_t           defaultValue) const;
    template<> char32_t             VarManager::ExtractValue(const std::string& varName, char32_t           defaultValue) const;
    template<> wchar_t              VarManager::ExtractValue(const std::string& varName, wchar_t            defaultValue) const;
    template<> signed char          VarManager::ExtractValue(const std::string& varName, signed char        defaultValue) const;
    template<> short                VarManager::ExtractValue(const std::string& varName, short              defaultValue) const;
    template<> int                  VarManager::ExtractValue(const std::string& varName, int                defaultValue) const;
    template<> long                 VarManager::ExtractValue(const std::string& varName, long               defaultValue) const;
    template<> long long            VarManager::ExtractValue(const std::string& varName, long long          defaultValue) const;
    template<> unsigned char        VarManager::ExtractValue(const std::string& varName, unsigned char      defaultValue) const;
    template<> unsigned short       VarManager::ExtractValue(const std::string& varName, unsigned short     defaultValue) const;
    template<> unsigned int         VarManager::ExtractValue(const std::string& varName, unsigned int       defaultValue) const;
    template<> unsigned long        VarManager::ExtractValue(const std::string& varName, unsigned long      defaultValue) const;
    template<> unsigned long long   VarManager::ExtractValue(const std::string& varName, unsigned long long defaultValue) const;
    template<> void*                VarManager::ExtractValue(const std::string& varName, void*              defaultValue) const;
    //
    // Arrays
    //
    template<> bool*                 VarManager::ExtractValue(const std::string& varName, bool*               defaultValue) const;
    template<> float*                VarManager::ExtractValue(const std::string& varName, float*              defaultValue) const;
    template<> double*               VarManager::ExtractValue(const std::string& varName, double*             defaultValue) const;
    template<> long double*          VarManager::ExtractValue(const std::string& varName, long double*        defaultValue) const;
    template<> char*                 VarManager::ExtractValue(const std::string& varName, char*               defaultValue) const;
    template<> char16_t*             VarManager::ExtractValue(const std::string& varName, char16_t*           defaultValue) const;
    template<> char32_t*             VarManager::ExtractValue(const std::string& varName, char32_t*           defaultValue) const;
    template<> wchar_t*              VarManager::ExtractValue(const std::string& varName, wchar_t*            defaultValue) const;
    template<> signed char*          VarManager::ExtractValue(const std::string& varName, signed char*        defaultValue) const;
    template<> short*                VarManager::ExtractValue(const std::string& varName, short*              defaultValue) const;
    template<> int*                  VarManager::ExtractValue(const std::string& varName, int*                defaultValue) const;
    template<> long*                 VarManager::ExtractValue(const std::string& varName, long*               defaultValue) const;
    template<> long long*            VarManager::ExtractValue(const std::string& varName, long long*          defaultValue) const;
    template<> unsigned char*        VarManager::ExtractValue(const std::string& varName, unsigned char*      defaultValue) const;
    template<> unsigned short*       VarManager::ExtractValue(const std::string& varName, unsigned short*     defaultValue) const;
    template<> unsigned int*         VarManager::ExtractValue(const std::string& varName, unsigned int*       defaultValue) const;
    template<> unsigned long*        VarManager::ExtractValue(const std::string& varName, unsigned long*      defaultValue) const;
    template<> unsigned long long*   VarManager::ExtractValue(const std::string& varName, unsigned long long* defaultValue) const;
}
