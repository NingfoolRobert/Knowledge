// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Test.proto

#include "Test.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
extern PROTOBUF_INTERNAL_EXPORT_Test_2eproto ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_helloworld_ListTestEntry_DoNotUse_Test_2eproto;
extern PROTOBUF_INTERNAL_EXPORT_Test_2eproto ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_person_Test_2eproto;
namespace lm {
class personDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<person> _instance;
} _person_default_instance_;
class helloworld_ListTestEntry_DoNotUseDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<helloworld_ListTestEntry_DoNotUse> _instance;
} _helloworld_ListTestEntry_DoNotUse_default_instance_;
class helloworldDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<helloworld> _instance;
} _helloworld_default_instance_;
}  // namespace lm
static void InitDefaultsscc_info_helloworld_Test_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::lm::_helloworld_default_instance_;
    new (ptr) ::lm::helloworld();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::lm::helloworld::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<2> scc_info_helloworld_Test_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 2, 0, InitDefaultsscc_info_helloworld_Test_2eproto}, {
      &scc_info_helloworld_ListTestEntry_DoNotUse_Test_2eproto.base,
      &scc_info_person_Test_2eproto.base,}};

static void InitDefaultsscc_info_helloworld_ListTestEntry_DoNotUse_Test_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::lm::_helloworld_ListTestEntry_DoNotUse_default_instance_;
    new (ptr) ::lm::helloworld_ListTestEntry_DoNotUse();
  }
  ::lm::helloworld_ListTestEntry_DoNotUse::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_helloworld_ListTestEntry_DoNotUse_Test_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_helloworld_ListTestEntry_DoNotUse_Test_2eproto}, {}};

static void InitDefaultsscc_info_person_Test_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::lm::_person_default_instance_;
    new (ptr) ::lm::person();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::lm::person::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_person_Test_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_person_Test_2eproto}, {}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_Test_2eproto[3];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_Test_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_Test_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_Test_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::lm::person, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::lm::person, id_),
  PROTOBUF_FIELD_OFFSET(::lm::person, name_),
  PROTOBUF_FIELD_OFFSET(::lm::helloworld_ListTestEntry_DoNotUse, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::lm::helloworld_ListTestEntry_DoNotUse, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::lm::helloworld_ListTestEntry_DoNotUse, key_),
  PROTOBUF_FIELD_OFFSET(::lm::helloworld_ListTestEntry_DoNotUse, value_),
  0,
  1,
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::lm::helloworld, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::lm::helloworld, id_),
  PROTOBUF_FIELD_OFFSET(::lm::helloworld, str_),
  PROTOBUF_FIELD_OFFSET(::lm::helloworld, list_test_),
  PROTOBUF_FIELD_OFFSET(::lm::helloworld, list_array_),
  PROTOBUF_FIELD_OFFSET(::lm::helloworld, list_person_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::lm::person)},
  { 7, 14, sizeof(::lm::helloworld_ListTestEntry_DoNotUse)},
  { 16, -1, sizeof(::lm::helloworld)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::lm::_person_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::lm::_helloworld_ListTestEntry_DoNotUse_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::lm::_helloworld_default_instance_),
};

const char descriptor_table_protodef_Test_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\nTest.proto\022\002lm\"\"\n\006person\022\n\n\002id\030\001 \001(\005\022\014"
  "\n\004name\030\002 \001(\t\"\274\001\n\nhelloworld\022\n\n\002id\030\001 \001(\005\022"
  "\013\n\003str\030\002 \001(\t\022/\n\tlist_Test\030\003 \003(\0132\034.lm.hel"
  "loworld.ListTestEntry\022\022\n\nlist_array\030\004 \003("
  "\005\022\037\n\013list_person\030\005 \003(\0132\n.lm.person\032/\n\rLi"
  "stTestEntry\022\013\n\003key\030\001 \001(\005\022\r\n\005value\030\002 \001(\005:"
  "\0028\001b\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_Test_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_Test_2eproto_sccs[3] = {
  &scc_info_helloworld_Test_2eproto.base,
  &scc_info_helloworld_ListTestEntry_DoNotUse_Test_2eproto.base,
  &scc_info_person_Test_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_Test_2eproto_once;
static bool descriptor_table_Test_2eproto_initialized = false;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_Test_2eproto = {
  &descriptor_table_Test_2eproto_initialized, descriptor_table_protodef_Test_2eproto, "Test.proto", 251,
  &descriptor_table_Test_2eproto_once, descriptor_table_Test_2eproto_sccs, descriptor_table_Test_2eproto_deps, 3, 0,
  schemas, file_default_instances, TableStruct_Test_2eproto::offsets,
  file_level_metadata_Test_2eproto, 3, file_level_enum_descriptors_Test_2eproto, file_level_service_descriptors_Test_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_Test_2eproto = (  ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_Test_2eproto), true);
namespace lm {

// ===================================================================

void person::InitAsDefaultInstance() {
}
class person::_Internal {
 public:
};

person::person()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:lm.person)
}
person::person(const person& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  name_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_name().empty()) {
    name_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.name_);
  }
  id_ = from.id_;
  // @@protoc_insertion_point(copy_constructor:lm.person)
}

void person::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_person_Test_2eproto.base);
  name_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  id_ = 0;
}

person::~person() {
  // @@protoc_insertion_point(destructor:lm.person)
  SharedDtor();
}

void person::SharedDtor() {
  name_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void person::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const person& person::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_person_Test_2eproto.base);
  return *internal_default_instance();
}


void person::Clear() {
// @@protoc_insertion_point(message_clear_start:lm.person)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  name_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  id_ = 0;
  _internal_metadata_.Clear();
}

#if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
const char* person::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // int32 id = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          id_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // string name = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 18)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParserUTF8(_internal_mutable_name(), ptr, ctx, "lm.person.name");
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}
#else  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
bool person::MergePartialFromCodedStream(
    ::PROTOBUF_NAMESPACE_ID::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!PROTOBUF_PREDICT_TRUE(EXPRESSION)) goto failure
  ::PROTOBUF_NAMESPACE_ID::uint32 tag;
  // @@protoc_insertion_point(parse_start:lm.person)
  for (;;) {
    ::std::pair<::PROTOBUF_NAMESPACE_ID::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // int32 id = 1;
      case 1: {
        if (static_cast< ::PROTOBUF_NAMESPACE_ID::uint8>(tag) == (8 & 0xFF)) {

          DO_((::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::ReadPrimitive<
                   ::PROTOBUF_NAMESPACE_ID::int32, ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_INT32>(
                 input, &id_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // string name = 2;
      case 2: {
        if (static_cast< ::PROTOBUF_NAMESPACE_ID::uint8>(tag) == (18 & 0xFF)) {
          DO_(::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::ReadString(
                input, this->_internal_mutable_name()));
          DO_(::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
            this->_internal_name().data(), static_cast<int>(this->_internal_name().length()),
            ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::PARSE,
            "lm.person.name"));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::PROTOBUF_NAMESPACE_ID::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:lm.person)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:lm.person)
  return false;
#undef DO_
}
#endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER

::PROTOBUF_NAMESPACE_ID::uint8* person::InternalSerializeWithCachedSizesToArray(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:lm.person)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 id = 1;
  if (this->id() != 0) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(1, this->id(), target);
  }

  // string name = 2;
  if (this->name().size() > 0) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_name().data(), static_cast<int>(this->_internal_name().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "lm.person.name");
    target = stream->WriteStringMaybeAliased(
        2, this->_internal_name(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:lm.person)
  return target;
}

size_t person::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:lm.person)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string name = 2;
  if (this->name().size() > 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_name());
  }

  // int32 id = 1;
  if (this->id() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->id());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void person::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:lm.person)
  GOOGLE_DCHECK_NE(&from, this);
  const person* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<person>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:lm.person)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:lm.person)
    MergeFrom(*source);
  }
}

void person::MergeFrom(const person& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:lm.person)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.name().size() > 0) {

    name_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.name_);
  }
  if (from.id() != 0) {
    set_id(from.id());
  }
}

void person::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:lm.person)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void person::CopyFrom(const person& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:lm.person)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool person::IsInitialized() const {
  return true;
}

void person::InternalSwap(person* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  name_.Swap(&other->name_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  swap(id_, other->id_);
}

::PROTOBUF_NAMESPACE_ID::Metadata person::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

helloworld_ListTestEntry_DoNotUse::helloworld_ListTestEntry_DoNotUse() {}
helloworld_ListTestEntry_DoNotUse::helloworld_ListTestEntry_DoNotUse(::PROTOBUF_NAMESPACE_ID::Arena* arena)
    : SuperType(arena) {}
void helloworld_ListTestEntry_DoNotUse::MergeFrom(const helloworld_ListTestEntry_DoNotUse& other) {
  MergeFromInternal(other);
}
::PROTOBUF_NAMESPACE_ID::Metadata helloworld_ListTestEntry_DoNotUse::GetMetadata() const {
  return GetMetadataStatic();
}
void helloworld_ListTestEntry_DoNotUse::MergeFrom(
    const ::PROTOBUF_NAMESPACE_ID::Message& other) {
  ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom(other);
}


// ===================================================================

void helloworld::InitAsDefaultInstance() {
}
class helloworld::_Internal {
 public:
};

helloworld::helloworld()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:lm.helloworld)
}
helloworld::helloworld(const helloworld& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr),
      list_array_(from.list_array_),
      list_person_(from.list_person_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  list_test_.MergeFrom(from.list_test_);
  str_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_str().empty()) {
    str_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.str_);
  }
  id_ = from.id_;
  // @@protoc_insertion_point(copy_constructor:lm.helloworld)
}

void helloworld::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_helloworld_Test_2eproto.base);
  str_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  id_ = 0;
}

helloworld::~helloworld() {
  // @@protoc_insertion_point(destructor:lm.helloworld)
  SharedDtor();
}

void helloworld::SharedDtor() {
  str_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void helloworld::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const helloworld& helloworld::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_helloworld_Test_2eproto.base);
  return *internal_default_instance();
}


void helloworld::Clear() {
// @@protoc_insertion_point(message_clear_start:lm.helloworld)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  list_test_.Clear();
  list_array_.Clear();
  list_person_.Clear();
  str_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  id_ = 0;
  _internal_metadata_.Clear();
}

#if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
const char* helloworld::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // int32 id = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          id_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // string str = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 18)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParserUTF8(_internal_mutable_str(), ptr, ctx, "lm.helloworld.str");
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // map<int32, int32> list_Test = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 26)) {
          ptr -= 1;
          do {
            ptr += 1;
            ptr = ctx->ParseMessage(&list_test_, ptr);
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<26>(ptr));
        } else goto handle_unusual;
        continue;
      // repeated int32 list_array = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 34)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::PackedInt32Parser(mutable_list_array(), ptr, ctx);
          CHK_(ptr);
        } else if (static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 32) {
          add_list_array(::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // repeated .lm.person list_person = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 42)) {
          ptr -= 1;
          do {
            ptr += 1;
            ptr = ctx->ParseMessage(add_list_person(), ptr);
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<42>(ptr));
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}
#else  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
bool helloworld::MergePartialFromCodedStream(
    ::PROTOBUF_NAMESPACE_ID::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!PROTOBUF_PREDICT_TRUE(EXPRESSION)) goto failure
  ::PROTOBUF_NAMESPACE_ID::uint32 tag;
  // @@protoc_insertion_point(parse_start:lm.helloworld)
  for (;;) {
    ::std::pair<::PROTOBUF_NAMESPACE_ID::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // int32 id = 1;
      case 1: {
        if (static_cast< ::PROTOBUF_NAMESPACE_ID::uint8>(tag) == (8 & 0xFF)) {

          DO_((::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::ReadPrimitive<
                   ::PROTOBUF_NAMESPACE_ID::int32, ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_INT32>(
                 input, &id_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // string str = 2;
      case 2: {
        if (static_cast< ::PROTOBUF_NAMESPACE_ID::uint8>(tag) == (18 & 0xFF)) {
          DO_(::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::ReadString(
                input, this->_internal_mutable_str()));
          DO_(::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
            this->_internal_str().data(), static_cast<int>(this->_internal_str().length()),
            ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::PARSE,
            "lm.helloworld.str"));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // map<int32, int32> list_Test = 3;
      case 3: {
        if (static_cast< ::PROTOBUF_NAMESPACE_ID::uint8>(tag) == (26 & 0xFF)) {
          helloworld_ListTestEntry_DoNotUse::Parser< ::PROTOBUF_NAMESPACE_ID::internal::MapField<
              helloworld_ListTestEntry_DoNotUse,
              ::PROTOBUF_NAMESPACE_ID::int32, ::PROTOBUF_NAMESPACE_ID::int32,
              ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_INT32,
              ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_INT32,
              0 >,
            ::PROTOBUF_NAMESPACE_ID::Map< ::PROTOBUF_NAMESPACE_ID::int32, ::PROTOBUF_NAMESPACE_ID::int32 > > parser(&list_test_);
          DO_(::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::ReadMessageNoVirtual(
              input, &parser));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // repeated int32 list_array = 4;
      case 4: {
        if (static_cast< ::PROTOBUF_NAMESPACE_ID::uint8>(tag) == (34 & 0xFF)) {
          DO_((::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::ReadPackedPrimitive<
                   ::PROTOBUF_NAMESPACE_ID::int32, ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_INT32>(
                 input, this->mutable_list_array())));
        } else if (static_cast< ::PROTOBUF_NAMESPACE_ID::uint8>(tag) == (32 & 0xFF)) {
          DO_((::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::ReadRepeatedPrimitiveNoInline<
                   ::PROTOBUF_NAMESPACE_ID::int32, ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_INT32>(
                 1, 34u, input, this->mutable_list_array())));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // repeated .lm.person list_person = 5;
      case 5: {
        if (static_cast< ::PROTOBUF_NAMESPACE_ID::uint8>(tag) == (42 & 0xFF)) {
          DO_(::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::ReadMessage(
                input, add_list_person()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::PROTOBUF_NAMESPACE_ID::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:lm.helloworld)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:lm.helloworld)
  return false;
#undef DO_
}
#endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER

::PROTOBUF_NAMESPACE_ID::uint8* helloworld::InternalSerializeWithCachedSizesToArray(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:lm.helloworld)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 id = 1;
  if (this->id() != 0) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(1, this->id(), target);
  }

  // string str = 2;
  if (this->str().size() > 0) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_str().data(), static_cast<int>(this->_internal_str().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "lm.helloworld.str");
    target = stream->WriteStringMaybeAliased(
        2, this->_internal_str(), target);
  }

  // map<int32, int32> list_Test = 3;
  if (!this->list_test().empty()) {
    typedef ::PROTOBUF_NAMESPACE_ID::Map< ::PROTOBUF_NAMESPACE_ID::int32, ::PROTOBUF_NAMESPACE_ID::int32 >::const_pointer
        ConstPtr;
    typedef ::PROTOBUF_NAMESPACE_ID::internal::SortItem< ::PROTOBUF_NAMESPACE_ID::int32, ConstPtr > SortItem;
    typedef ::PROTOBUF_NAMESPACE_ID::internal::CompareByFirstField<SortItem> Less;

    if (stream->IsSerializationDeterministic() &&
        this->list_test().size() > 1) {
      ::std::unique_ptr<SortItem[]> items(
          new SortItem[this->list_test().size()]);
      typedef ::PROTOBUF_NAMESPACE_ID::Map< ::PROTOBUF_NAMESPACE_ID::int32, ::PROTOBUF_NAMESPACE_ID::int32 >::size_type size_type;
      size_type n = 0;
      for (::PROTOBUF_NAMESPACE_ID::Map< ::PROTOBUF_NAMESPACE_ID::int32, ::PROTOBUF_NAMESPACE_ID::int32 >::const_iterator
          it = this->list_test().begin();
          it != this->list_test().end(); ++it, ++n) {
        items[static_cast<ptrdiff_t>(n)] = SortItem(&*it);
      }
      ::std::sort(&items[0], &items[static_cast<ptrdiff_t>(n)], Less());
      for (size_type i = 0; i < n; i++) {
        target = helloworld_ListTestEntry_DoNotUse::Funcs::InternalSerialize(3, items[static_cast<ptrdiff_t>(i)].second->first, items[static_cast<ptrdiff_t>(i)].second->second, target, stream);
      }
    } else {
      for (::PROTOBUF_NAMESPACE_ID::Map< ::PROTOBUF_NAMESPACE_ID::int32, ::PROTOBUF_NAMESPACE_ID::int32 >::const_iterator
          it = this->list_test().begin();
          it != this->list_test().end(); ++it) {
        target = helloworld_ListTestEntry_DoNotUse::Funcs::InternalSerialize(3, it->first, it->second, target, stream);
      }
    }
  }

  // repeated int32 list_array = 4;
  {
    int byte_size = _list_array_cached_byte_size_.load(std::memory_order_relaxed);
    if (byte_size > 0) {
      target = stream->WriteInt32Packed(
          4, list_array_, byte_size, target);
    }
  }

  // repeated .lm.person list_person = 5;
  for (auto it = this->list_person_.pointer_begin(),
            end = this->list_person_.pointer_end(); it < end; ++it) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessageToArray(5, **it, target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:lm.helloworld)
  return target;
}

size_t helloworld::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:lm.helloworld)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // map<int32, int32> list_Test = 3;
  total_size += 1 *
      ::PROTOBUF_NAMESPACE_ID::internal::FromIntSize(this->list_test_size());
  for (::PROTOBUF_NAMESPACE_ID::Map< ::PROTOBUF_NAMESPACE_ID::int32, ::PROTOBUF_NAMESPACE_ID::int32 >::const_iterator
      it = this->list_test().begin();
      it != this->list_test().end(); ++it) {
    total_size += helloworld_ListTestEntry_DoNotUse::Funcs::ByteSizeLong(it->first, it->second);
  }

  // repeated int32 list_array = 4;
  {
    size_t data_size = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      Int32Size(this->list_array_);
    if (data_size > 0) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
            static_cast<::PROTOBUF_NAMESPACE_ID::int32>(data_size));
    }
    int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(data_size);
    _list_array_cached_byte_size_.store(cached_size,
                                    std::memory_order_relaxed);
    total_size += data_size;
  }

  // repeated .lm.person list_person = 5;
  total_size += 1UL * this->list_person_size();
  for (const auto& msg : this->list_person_) {
    total_size +=
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(msg);
  }

  // string str = 2;
  if (this->str().size() > 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_str());
  }

  // int32 id = 1;
  if (this->id() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->id());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void helloworld::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:lm.helloworld)
  GOOGLE_DCHECK_NE(&from, this);
  const helloworld* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<helloworld>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:lm.helloworld)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:lm.helloworld)
    MergeFrom(*source);
  }
}

void helloworld::MergeFrom(const helloworld& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:lm.helloworld)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  list_test_.MergeFrom(from.list_test_);
  list_array_.MergeFrom(from.list_array_);
  list_person_.MergeFrom(from.list_person_);
  if (from.str().size() > 0) {

    str_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.str_);
  }
  if (from.id() != 0) {
    set_id(from.id());
  }
}

void helloworld::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:lm.helloworld)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void helloworld::CopyFrom(const helloworld& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:lm.helloworld)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool helloworld::IsInitialized() const {
  return true;
}

void helloworld::InternalSwap(helloworld* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  list_test_.Swap(&other->list_test_);
  list_array_.InternalSwap(&other->list_array_);
  list_person_.InternalSwap(&other->list_person_);
  str_.Swap(&other->str_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  swap(id_, other->id_);
}

::PROTOBUF_NAMESPACE_ID::Metadata helloworld::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace lm
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::lm::person* Arena::CreateMaybeMessage< ::lm::person >(Arena* arena) {
  return Arena::CreateInternal< ::lm::person >(arena);
}
template<> PROTOBUF_NOINLINE ::lm::helloworld_ListTestEntry_DoNotUse* Arena::CreateMaybeMessage< ::lm::helloworld_ListTestEntry_DoNotUse >(Arena* arena) {
  return Arena::CreateInternal< ::lm::helloworld_ListTestEntry_DoNotUse >(arena);
}
template<> PROTOBUF_NOINLINE ::lm::helloworld* Arena::CreateMaybeMessage< ::lm::helloworld >(Arena* arena) {
  return Arena::CreateInternal< ::lm::helloworld >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
