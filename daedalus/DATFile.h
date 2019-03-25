#pragma once
#include <map>
#include <string>
#include <vector>
#include <daedalus/DaedalusStdlib.h>
#include <utils/logger.h>
#include <utils/staticReferencedAllocator.h>
#include <zenload/zenParser.h>

namespace Daedalus {
enum EInstanceClass {
  IC_Npc,
  IC_Mission,
  IC_Info,
  IC_Item,
  IC_ItemReact,
  IC_Focus,
  IC_Menu,
  IC_MenuItem,
  IC_Sfx,
  IC_Pfx,
  IC_MusicTheme,
  IC_GilValues,
  IC_FightAi
  };

template <class C_Class>
EInstanceClass enumFromClass();

enum EParFlag
  {
  EParFlag_Const = 1 << 0,
  EParFlag_Return = 1 << 1,
  EParFlag_ClassVar = 1 << 2,
  EParFlag_External = 1 << 3,
  EParFlag_Merged = 1 << 4
  };

enum EParType
  {
  EParType_Void = 0,
  EParType_Float = 1,
  EParType_Int = 2,
  EParType_String = 3,
  EParType_Class = 4,
  EParType_Func = 5,
  EParType_Prototype = 6,
  EParType_Instance = 7
  };

enum EParOp : uint8_t
  {
  EParOp_Add = 0,              // a + b
  EParOp_Subract = 1,          // a - b
  EParOp_Multiply = 2,         // a * b
  EParOp_Divide = 3,           // a / b
  EParOp_Mod = 4,              // a % b
  EParOp_BinOr = 5,            // a | b
  EParOp_BinAnd = 6,           // a & b
  EParOp_Less = 7,             // a < b
  EParOp_Greater = 8,          // a > b
  EParOp_Assign = 9,           // a = b
  EParOp_LogOr = 11,           // a || b
  EParOp_LogAnd = 12,          // a && b
  EParOp_ShiftLeft = 13,       // a << b
  EParOp_ShiftRight = 14,      // a >> b
  EParOp_LessOrEqual = 15,     // a <= b
  EParOp_Equal = 16,           // a == b
  EParOp_NotEqual = 17,        // a != b
  EParOp_GreaterOrEqual = 18,  // a >= b
  EParOp_AssignAdd = 19,       // a += b (a = a + b)
  EParOp_AssignSubtract = 20,  // a -= b (a = a - b)
  EParOp_AssignMultiply = 21,  // a *= b (a = a * b)
  EParOp_AssignDivide = 22,    // a /= b (a = a / b)
  EParOp_Plus = 30,            // +a
  EParOp_Minus = 31,           // -a
  EParOp_Not = 32,             // !a
  EParOp_Negate = 33,          // ~a
  //	EParOp_LeftBracket     = 40,    // '('
  //	EParOp_RightBracket    = 41,    // ')'
  //	EParOp_Semicolon       = 42,    // ';'
  //	EParOp_Comma           = 43,    // ','
  //	EParOp_CurlyBracket    = 44,    // '{', '}'
  //	EParOp_None            = 45,
  //	EParOp_Float           = 51,
  //	EParOp_Var             = 52,
  //	EParOp_Operator        = 53,
  EParOp_Ret = 60,
  EParOp_Call = 61,
  EParOp_CallExternal = 62,
  //	EParOp_PopInt          = 63,
  EParOp_PushInt = 64,
  EParOp_PushVar = 65,
  //	EParOp_PushString      = 66,
  EParOp_PushInstance = 67,
  //	EParOp_PushIndex       = 68,
  //	EParOp_PopVar          = 69,
  EParOp_AssignString = 70,
  EParOp_AssignStringRef = 71,
  EParOp_AssignFunc = 72,
  EParOp_AssignFloat = 73,
  EParOp_AssignInstance = 74,
  EParOp_Jump = 75,
  EParOp_JumpIf = 76,
  EParOp_SetInstance = 80,
  //	EParOp_Skip            = 90,
  //	EParOp_Label           = 91,
  //	EParOp_Func            = 92,
  //	EParOp_FuncEnd         = 93,
  //	EParOp_Class           = 94,
  //	EParOp_ClassEnd        = 95,
  //	EParOp_Instance        = 96,
  //	EParOp_InstanceEnd     = 97,
  //	EParOp_String          = 98,
  //	EParOp_Array           = 180,  // EParOp_Var + 128
  EParOp_PushArrayVar = 245  // EParOp_PushVar + EParOp_Array
  };

struct PARSymbol {
  PARSymbol() {
    classMemberOffset = -1;
    classMemberArraySize = 0;
    instanceDataClass = IC_Npc;
    memset(&properties, 0, sizeof(properties));
    /*classOffset = 0;
            address = 0;
            classMemberOffset = 0;*/
    parent = 0xFFFFFFFF;
    }

  std::string name;

  struct Properties
    {
    int32_t offClsRet;  // Offset (ClassVar) | Size (Class) | ReturnType (Func)
    struct
      {
      uint32_t count : 12;  // Count:12, Type:4 (EParType_), Flags:6 (EParFlag_), Space: 1, Reserved:9
      uint32_t type : 4;    // EParType_*
      uint32_t flags : 6;   // EParFlag_*
      uint32_t space : 1;
      uint32_t reserved : 9;
      } elemProps;

    struct
      {
      uint32_t value : 19;  // Value:19, Reserved:13
      uint32_t reserved : 13;
      } fileIndex;

    struct
      {
      uint32_t value : 19;  // Value:19, Reserved:13
      uint32_t reserved : 13;
      } lineStart;

    struct
      {
      uint32_t value : 19;  // Value:19, Reserved:13
      uint32_t reserved : 13;
      } lineCount;

    struct
      {
      uint32_t value : 24;  // Value:24, Reserved:8
      uint32_t reserved : 8;
      } charStart;

    struct
      {
      uint32_t value : 24;  // Value:24, Reserved:8
      uint32_t reserved : 8;
      } charCount;

    } properties;

  bool hasEParFlag(EParFlag eParFlag)
    {
    return static_cast<bool>(properties.elemProps.flags & eParFlag);
    }

  bool isEParType(EParType eParType)
    {
    return properties.elemProps.type == eParType;
    }

  std::vector<float>       floatData;
  std::vector<int32_t>     intData;
  std::vector<std::string> strData;
  int32_t classOffset;
  uint32_t address;

  // Defacto reflections. Offset of the class member to be able to access class members via name
  int32_t classMemberOffset;  // Not stored in files, only valid for classes to directly write to engine memory
  // Store array size of the class member var. 1 for scalar members. Useful for bounds checking.
  uint32_t classMemberArraySize;  // Not stored in files, only valid for classes to directly write to engine memory

  void*              instanceData;  // Not stored in files, only valid for classes to directly write to engine memory
  void*              instanceDataHandle=nullptr;
  EInstanceClass     instanceDataClass;

  uint32_t parent;  // 0xFFFFFFFF (-1) = none

  void warnIndexOutOfBounds(size_t index, size_t size)
    {
    LogWarn() << "DaedalusVM: index out of range for: " << name << "[" << size << "], index = " << index;
    }

  template <class T>
  T* getClassMember(void* baseAddr)
    {
    return reinterpret_cast<T*>(reinterpret_cast<char*>(baseAddr) + classMemberOffset);
    }

  int32_t& getInt(size_t idx = 0, void* baseAddr = nullptr);
  std::string& getString(size_t idx = 0, void* baseAddr = nullptr);
  float& getFloat(size_t idx = 0, void* baseAddr = nullptr);

  template <typename T>
  std::vector<T>& getDataContainer();

  template <class T>
  T& getValue(size_t idx = 0, void* baseAddr = nullptr) {
    bool isClassVar = this->hasEParFlag(EParFlag_ClassVar);
    if(isClassVar) {
      bool isRegistered = classMemberOffset != -1;
      if(!isRegistered) {
        LogError() << "DaedalusVM: class data member not registered: " << name;
        }
      else if (baseAddr == nullptr) {
        LogError() << "DaedalusVM: base address of C_Class is nullptr: " << name;
        }
      else if (idx >= classMemberArraySize) {
        warnIndexOutOfBounds(idx, classMemberArraySize);
        LogError() << "DaedalusVM: index out of range for registered class data member: " << name;
        }
      else {
        return getClassMember<T>(baseAddr)[idx];
        }
      }

    std::vector<T>& data = getDataContainer<T>();
    // read from symbol's data if not isClassVar or the above failed. (the latter should not happen)
    if (data.size() <= idx)
      {
      if (!isClassVar)  // only print error message if we did not fall through from above
        warnIndexOutOfBounds(idx, data.size());
      data.resize(idx + 1);
      }
    return data[idx];
    }

  void set(int32_t v, size_t idx = 0, void* baseAddr = nullptr)
    {
    switch (properties.elemProps.type)
      {
      case EParType_Func:
        address = v;

        if (baseAddr && classMemberOffset != -1)
          *reinterpret_cast<int32_t*>(reinterpret_cast<char*>(baseAddr) + classMemberOffset + (sizeof(int32_t) * idx)) = v;
        break;
      default:
        assert(false);
      }
    }

  bool isDataSame(const PARSymbol& other) const
    {
    return intData == other.intData && floatData == other.floatData && strData == other.strData;
    }
  };

struct PARSymTable {
  std::vector<uint32_t>                       sortTable;
  std::vector<PARSymbol>                      symbols;
  std::vector<std::pair<std::string, size_t>> symbolsByName;
  std::unordered_map<size_t, size_t>          functionsByAddress;
  };

struct PARStack {
  size_t stackOffset;
  size_t stackSize;
  };

struct PARStackOpCode {
  EParOp op;

  /**
   * These are valid depending on "op"
   */
  union {
    int32_t address;  // EParOp_Call, EParOp_Jump, EParOp_JumpIf,
    int32_t symbol;   // EParOp_CallExternal, EParOp_PushVar, EParOp_PushInstance, EParOp_SetInstance, EParOp_PushArrayVar
    int32_t value;    // EParOp_PushInt
    };
  uint8_t index;    // EParOp_PushArrayVar
  size_t  opSize;  // Size of this operation
  };

struct PARInstance {
  std::map<std::string, uint32_t> symbolsByName;
  };

class DATFile {
  public:
    DATFile()=default;
    DATFile(const uint8_t* pData, size_t numBytes);

    bool           hasSymbolName       (const char* symName);
    PARSymbol&     getSymbolByName     (const char* symName);
    size_t         getSymbolIndexByName(const char* symName);
    PARSymbol&     getSymbolByIndex    (size_t idx);

    size_t         getFunctionIndexByAddress(size_t address);
    void           iterateSymbolsOfClass(const char *className, std::function<void(size_t, PARSymbol&)> callback);
    const PARStackOpCode &getStackOpCode(size_t pc);
    size_t         addSymbol();

    const PARSymTable& getSymTable() { return m_SymTable; }

    template<class T,class M>
    void registerMember(const char* name,T M::* field, bool checkIfExists){
      if(checkIfExists && !hasSymbolName(name))
        return;
      auto& parSymbol                = getSymbolByName(name);
      parSymbol.classMemberOffset    = offsetOf(field);
      parSymbol.classMemberArraySize = std::max<size_t>(1u,std::extent<T>());
      }

  private:
    void readSymTable(ZenLoad::ZenParser& input);
    void readStack   (ZenLoad::ZenParser& input);

    template<class T,class M>
    static ptrdiff_t offsetOf(T M::* field){
      char        tmp = 0;
      const char* f   = reinterpret_cast<const char*>(&(reinterpret_cast<M*>(&tmp)->*field));
      return reinterpret_cast<ptrdiff_t>(f-&tmp);
      }

    static int compareNoCase(const char* a,const char* b);

    PARSymTable                  m_SymTable;

    std::vector<PARStackOpCode>  code;
    std::vector<PARStackOpCode*> pcCode;
  };

}  // namespace Daedalus
