#include "ZString.h"

#include <zenload/zenParser.h>

using namespace Daedalus;

static std::shared_ptr<std::string> emptyStr() {
  static std::shared_ptr<std::string> v = std::make_shared<std::string>();
  return v;
  }

static ZString strFromIntCache(uint8_t v){
  static ZString intCache[256];
  if(intCache[v].empty()){
    char s[32]={};
    std::snprintf(s,sizeof(s),"%d",v);
    intCache[v] = ZString(s);
    }
  return intCache[v];
  }

ZString::ZString()
  :val(emptyStr()) {
  }

ZString::ZString(const char* s) {
  if(s==nullptr || s[0]=='\0')
    val = emptyStr(); else
    val = std::make_shared<std::string>(s);
  }

ZString::ZString(std::string&& s)
  :val(std::make_shared<std::string>(std::move(s))){
  }

ZString::ZString(std::unique_ptr<char[]>&& s)
  :val(std::make_shared<std::string>(s.get())){
  }

ZString::ZString(ZenLoad::ZenParser &input) {
  size_t       size = 0;

  size_t  pos = input.getSeek();
  uint8_t b   = input.readBinaryByte();
  while(b != 0x0A) {
    size++;
    b = input.readBinaryByte();
    }
  input.setSeek(pos);

  if(size>0) {
    val = std::make_shared<std::string>();
    std::string& s    = *val;
    s.resize(size);

    input.readBinaryRaw(&s[0],size);
    size=0;

    char* at = &s[0];
    for(size_t i=0;i<s.size();) {
      if(s[i]=='\\' && (i+1<s.size()) && s[i+1]=='n'){
        at[size] = '\n';
        i+=2;
        size++;
        } else
      if(uint8_t(s[i])==0xFF){
        // FIXME: This happens at INSTANCE_HELP?
        i++;
        } else {
        at[size] = s[i];
        i++;
        size++;
        }
      }
    s.resize(size);
    } else {
    val = emptyStr();
    }

  input.readBinaryByte(); // skip 0x0A
  }

ZString::ZString(ZString&& other)
  :val(other.val){
  other.val = emptyStr();
  }

ZString& ZString::operator =(ZString&& other) {
  std::swap(val,other.val);
  return *this;
  }

bool ZString::operator == (const char* r) const {
  return std::strcmp(c_str(),r)==0;
  }

bool ZString::operator !=(const char* r) const {
  return std::strcmp(c_str(),r)!=0;
  }

bool ZString::operator == (const ZString &r) const {
  if(val==r.val)
    return true;
  return *val==*r.val;
  }

bool ZString::operator !=(const ZString &r) const {
  return !(*this==r);
  }

bool ZString::operator <(const ZString& r) const {
  return (*val<*r.val);
  }

bool ZString::operator <=(const ZString& r) const {
  return (*val<=*r.val);
  }

bool ZString::operator >(const ZString& r) const {
  return (*val>*r.val);
  }

bool ZString::operator >=(const ZString& r) const {
  return (*val>=*r.val);
  }

bool ZString::operator <(const char* r) const {
  return (*val<r);
  }

bool ZString::operator <=(const char* r) const {
  return (*val<=r);
  }

bool ZString::operator >(const char* r) const {
  return (*val>r);
  }

bool ZString::operator >=(const char* r) const {
  return (*val>=r);
  }

ZString ZString::operator + (const ZString& r) const {
  ZString ret;
  ret.val = std::make_shared<std::string>(*val+*r.val);
  return ret;
  }

size_t ZString::find(const char* src) const {
  return val->find(src);
  }

const char *ZString::c_str() const {
  return val->c_str();
  }

bool ZString::empty() const {
  return val->empty();
  }

size_t ZString::size() const {
  return val->size();
  }

ZString ZString::toStr(int32_t v) {
  if(v<256)
    return strFromIntCache(uint8_t(v));

  char s[32]={};
  std::snprintf(s,sizeof(s),"%d",v);
  return ZString(s);
  }

ZString ZString::toStr(int64_t v) {
  if(v<256)
    return strFromIntCache(uint8_t(v));

  char s[32]={};
  std::snprintf(s,sizeof(s),"%lld",v);
  return ZString(s);
  }

ZString ZString::toStr(float v) {
  char s[32]={};
  std::snprintf(s,sizeof(s),"%f",v);
  return ZString(s);
  }
