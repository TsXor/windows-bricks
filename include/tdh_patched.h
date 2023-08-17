#pragma once
#ifndef __TDH_PATCH_H__
#define __TDH_PATCH_H__

#include <minwinbase.h>
#include <minwindef.h>

#ifndef __GNUC__

#include <tdh.h>

#else // __GNUC__

#include <_mingw.h>

#if (_WIN32_WINNT >= 0x0600)

#ifdef __cplusplus
extern "C" {
#endif

// mingw-only patch for <tdh.h>
// according to https://learn.microsoft.com/en-us/windows/win32/api/tdh/

#include <evntprov.h>
#include <evntcons.h>

// very classic Microsoft Win32API style defs
typedef ULONG TDHSTATUS;
#define TDHAPI TDHSTATUS __stdcall
typedef HANDLE TDH_HANDLE, *PTDH_HANDLE;

//====================
// Enumerations
//====================

/*
    _TDH_IN_TYPE
    Defines the supported [in] types for a trace data helper (TDH).
*/
typedef enum _TDH_IN_TYPE {
    TDH_INTYPE_NULL,
    TDH_INTYPE_UNICODESTRING,
    TDH_INTYPE_ANSISTRING,
    TDH_INTYPE_INT8,
    TDH_INTYPE_UINT8,
    TDH_INTYPE_INT16,
    TDH_INTYPE_UINT16,
    TDH_INTYPE_INT32,
    TDH_INTYPE_UINT32,
    TDH_INTYPE_INT64,
    TDH_INTYPE_UINT64,
    TDH_INTYPE_FLOAT,
    TDH_INTYPE_DOUBLE,
    TDH_INTYPE_BOOLEAN,
    TDH_INTYPE_BINARY,
    TDH_INTYPE_GUID,
    TDH_INTYPE_POINTER,
    TDH_INTYPE_FILETIME,
    TDH_INTYPE_SYSTEMTIME,
    TDH_INTYPE_SID,
    TDH_INTYPE_HEXINT32,
    TDH_INTYPE_HEXINT64,
    TDH_INTYPE_MANIFEST_COUNTEDSTRING,
    TDH_INTYPE_MANIFEST_COUNTEDANSISTRING,
    TDH_INTYPE_RESERVED24,
    TDH_INTYPE_MANIFEST_COUNTEDBINARY,
    TDH_INTYPE_COUNTEDSTRING,
    TDH_INTYPE_COUNTEDANSISTRING,
    TDH_INTYPE_REVERSEDCOUNTEDSTRING,
    TDH_INTYPE_REVERSEDCOUNTEDANSISTRING,
    TDH_INTYPE_NONNULLTERMINATEDSTRING,
    TDH_INTYPE_NONNULLTERMINATEDANSISTRING,
    TDH_INTYPE_UNICODECHAR,
    TDH_INTYPE_ANSICHAR,
    TDH_INTYPE_SIZET,
    TDH_INTYPE_HEXDUMP,
    TDH_INTYPE_WBEMSID
} ;

/*
    _TDH_OUT_TYPE
    Defines the supported [out] types for a trace data helper (TDH).
*/
typedef enum _TDH_OUT_TYPE {
    TDH_OUTTYPE_NULL,
    TDH_OUTTYPE_STRING,
    TDH_OUTTYPE_DATETIME,
    TDH_OUTTYPE_BYTE,
    TDH_OUTTYPE_UNSIGNEDBYTE,
    TDH_OUTTYPE_SHORT,
    TDH_OUTTYPE_UNSIGNEDSHORT,
    TDH_OUTTYPE_INT,
    TDH_OUTTYPE_UNSIGNEDINT,
    TDH_OUTTYPE_LONG,
    TDH_OUTTYPE_UNSIGNEDLONG,
    TDH_OUTTYPE_FLOAT,
    TDH_OUTTYPE_DOUBLE,
    TDH_OUTTYPE_BOOLEAN,
    TDH_OUTTYPE_GUID,
    TDH_OUTTYPE_HEXBINARY,
    TDH_OUTTYPE_HEXINT8,
    TDH_OUTTYPE_HEXINT16,
    TDH_OUTTYPE_HEXINT32,
    TDH_OUTTYPE_HEXINT64,
    TDH_OUTTYPE_PID,
    TDH_OUTTYPE_TID,
    TDH_OUTTYPE_PORT,
    TDH_OUTTYPE_IPV4,
    TDH_OUTTYPE_IPV6,
    TDH_OUTTYPE_SOCKETADDRESS,
    TDH_OUTTYPE_CIMDATETIME,
    TDH_OUTTYPE_ETWTIME,
    TDH_OUTTYPE_XML,
    TDH_OUTTYPE_ERRORCODE,
    TDH_OUTTYPE_WIN32ERROR,
    TDH_OUTTYPE_NTSTATUS,
    TDH_OUTTYPE_HRESULT,
    TDH_OUTTYPE_CULTURE_INSENSITIVE_DATETIME,
    TDH_OUTTYPE_JSON,
    TDH_OUTTYPE_UTF8,
    TDH_OUTTYPE_PKCS7_WITH_TYPE_INFO,
    TDH_OUTTYPE_CODE_POINTER,
    TDH_OUTTYPE_DATETIME_UTC,
    TDH_OUTTYPE_REDUCEDSTRING,
    TDH_OUTTYPE_NOPRINT
} ;

/*
    DECODING_SOURCE
    Defines the source of the event data.
*/
typedef enum _DECODING_SOURCE {
    DecodingSourceXMLFile,
    DecodingSourceWbem,
    DecodingSourceWPP,
    DecodingSourceTlg,
    DecodingSourceMax
} DECODING_SOURCE;

/*
    EVENT_FIELD_TYPE
    Defines the provider information to retrieve.
*/
typedef enum _EVENT_FIELD_TYPE {
    EventKeywordInformation = 0,
    EventLevelInformation,
    EventChannelInformation,
    EventTaskInformation,
    EventOpcodeInformation,
    EventInformationMax
} EVENT_FIELD_TYPE;

/*
    MAP_FLAGS
    Defines constant values that indicate if the map is a value map, bitmap, or pattern map.
*/
typedef enum _MAP_FLAGS {
    EVENTMAP_INFO_FLAG_MANIFEST_VALUEMAP = 0x1,
    EVENTMAP_INFO_FLAG_MANIFEST_BITMAP = 0x2,
    EVENTMAP_INFO_FLAG_MANIFEST_PATTERNMAP = 0x4,
    EVENTMAP_INFO_FLAG_WBEM_VALUEMAP = 0x8,
    EVENTMAP_INFO_FLAG_WBEM_BITMAP = 0x10,
    EVENTMAP_INFO_FLAG_WBEM_FLAG = 0x20,
    EVENTMAP_INFO_FLAG_WBEM_NO_MAP = 0x40
} MAP_FLAGS;

/*
    MAP_VALUETYPE
    Defines if the value map value is in a ULONG data type or a string.
*/
typedef enum _MAP_VALUETYPE {
    EVENTMAP_ENTRY_VALUETYPE_ULONG,
    EVENTMAP_ENTRY_VALUETYPE_STRING
} MAP_VALUETYPE;

/*
    PAYLOAD_OPERATOR
    Defines the supported payload operators for a trace data helper (TDH).
*/
typedef enum _PAYLOAD_OPERATOR {
    PAYLOADFIELD_EQ,
    PAYLOADFIELD_NE,
    PAYLOADFIELD_LE,
    PAYLOADFIELD_GT,
    PAYLOADFIELD_LT,
    PAYLOADFIELD_GE,
    PAYLOADFIELD_BETWEEN,
    PAYLOADFIELD_NOTBETWEEN,
    PAYLOADFIELD_MODULO,
    PAYLOADFIELD_CONTAINS,
    PAYLOADFIELD_DOESNTCONTAIN,
    PAYLOADFIELD_IS,
    PAYLOADFIELD_ISNOT,
    PAYLOADFIELD_INVALID
} PAYLOAD_OPERATOR;

/*
    PROPERTY_FLAGS
    Defines if the property is contained in a structure or array.
*/
typedef enum _PROPERTY_FLAGS {
    PropertyStruct = 0x1,
    PropertyParamLength = 0x2,
    PropertyParamCount = 0x4,
    PropertyWBEMXmlFragment = 0x8,
    PropertyParamFixedLength = 0x10,
    PropertyParamFixedCount = 0x20,
    PropertyHasTags = 0x40,
    PropertyHasCustomSchema = 0x80
} PROPERTY_FLAGS;

/*
    TDH_CONTEXT_TYPE
    Defines the context type.
*/
typedef enum _TDH_CONTEXT_TYPE {
    TDH_CONTEXT_WPP_TMFFILE,
    TDH_CONTEXT_WPP_TMFSEARCHPATH,
    TDH_CONTEXT_WPP_GMT,
    TDH_CONTEXT_POINTERSIZE,
    TDH_CONTEXT_PDB_PATH,
    TDH_CONTEXT_MAXIMUM
} TDH_CONTEXT_TYPE;

/*
    TEMPLATE_FLAGS
    Defines constant values that indicates the layout of the event data.
*/
typedef enum _TEMPLATE_FLAGS {
    TEMPLATE_EVENT_DATA = 1,
    TEMPLATE_USER_DATA = 2,
    TEMPLATE_CONTROL_GUID = 4
} TEMPLATE_FLAGS;


//====================
// Structures
//====================

/*
    EVENT_MAP_ENTRY
    Defines a single value map entry.
*/
typedef struct _EVENT_MAP_ENTRY {
    ULONG OutputOffset;
    union {
        ULONG Value;
        ULONG InputOffset;
    };
} EVENT_MAP_ENTRY, *PEVENT_MAP_ENTRY;

/*
    EVENT_MAP_INFO
    Defines the metadata about the event map.
*/
typedef struct _EVENT_MAP_INFO {
    ULONG               NameOffset;
    MAP_FLAGS           Flag;
    ULONG               EntryCount;
    union {
        MAP_VALUETYPE   MapEntryValueType;
        ULONG           FormatStringOffset;
    };
    EVENT_MAP_ENTRY     MapEntryArray[ANYSIZE_ARRAY];
} EVENT_MAP_INFO, *PEVENT_MAP_INFO;

/*
    EVENT_PROPERTY_INFO
    Provides information about a single property of the event or filter.
*/
typedef struct _EVENT_PROPERTY_INFO {
    PROPERTY_FLAGS Flags;
    ULONG          NameOffset;
    union {
        struct {
        USHORT InType;
        USHORT OutType;
        ULONG  MapNameOffset;
        } nonStructType;
        struct {
        USHORT StructStartIndex;
        USHORT NumOfStructMembers;
        ULONG  padding;
        } structType;
        struct {
        USHORT InType;
        USHORT OutType;
        ULONG  CustomSchemaOffset;
        } customSchemaType;
    };
    union {
        USHORT count;
        USHORT countPropertyIndex;
    };
    union {
        USHORT length;
        USHORT lengthPropertyIndex;
    };
    union {
        ULONG Reserved;
        struct {
        ULONG Tags : 28;
        };
    };
} EVENT_PROPERTY_INFO, *PEVENT_PROPERTY_INFO;

/*
    PAYLOAD_FILTER_PREDICATE
    Defines an event payload filter predicate that describes how to filter on a single field in a trace session.
*/
typedef struct _PAYLOAD_FILTER_PREDICATE {
    LPWSTR FieldName;
    USHORT CompareOp;
    LPWSTR Value;
} PAYLOAD_FILTER_PREDICATE, *PPAYLOAD_FILTER_PREDICATE;

/*
    PROPERTY_DATA_DESCRIPTOR
    Defines the property to retrieve.
*/
typedef struct _PROPERTY_DATA_DESCRIPTOR {
    ULONGLONG PropertyName;
    ULONG     ArrayIndex;
    ULONG     Reserved;
} PROPERTY_DATA_DESCRIPTOR, *PPROPERTY_DATA_DESCRIPTOR;

/*
    PROVIDER_ENUMERATION_INFO
    Defines the array of providers that have registered a MOF or manifest on the computer.
*/
typedef struct _PROVIDER_ENUMERATION_INFO {
    ULONG               NumberOfProviders;
    ULONG               Reserved;
    TRACE_PROVIDER_INFO TraceProviderInfoArray[ANYSIZE_ARRAY];
} PROVIDER_ENUMERATION_INFO, *PPROVIDER_ENUMERATION_INFO;

/*
    PROVIDER_EVENT_INFO
    Defines an array of events in a provider manifest.
*/
typedef struct _PROVIDER_EVENT_INFO {
    ULONG            NumberOfEvents;
    ULONG            Reserved;
    EVENT_DESCRIPTOR EventDescriptorsArray[ANYSIZE_ARRAY];
} PROVIDER_EVENT_INFO, *PPROVIDER_EVENT_INFO;

/*
    PROVIDER_FIELD_INFO
    Defines the field information.
*/
typedef struct _PROVIDER_FIELD_INFO {
    ULONG     NameOffset;
    ULONG     DescriptionOffset;
    ULONGLONG Value;
} PROVIDER_FIELD_INFO, *PPROVIDER_FIELD_INFO;

/*
    PROVIDER_FIELD_INFOARRAY
    Defines metadata information about the requested field.
*/
typedef struct _PROVIDER_FIELD_INFOARRAY {
    ULONG               NumberOfElements;
    EVENT_FIELD_TYPE    FieldType;
    PROVIDER_FIELD_INFO FieldInfoArray[ANYSIZE_ARRAY];
} PROVIDER_FIELD_INFOARRAY, *PPROVIDER_FIELD_INFOARRAY;

/*
    PROVIDER_FILTER_INFO
    Defines a filter and its data.
*/
typedef struct _PROVIDER_FILTER_INFO {
    UCHAR               Id;
    UCHAR               Version;
    ULONG               MessageOffset;
    ULONG               Reserved;
    ULONG               PropertyCount;
    EVENT_PROPERTY_INFO EventPropertyInfoArray[ANYSIZE_ARRAY];
} PROVIDER_FILTER_INFO, *PPROVIDER_FILTER_INFO;

/*
    TDH_CONTEXT
    Defines the additional information required to parse an event.
*/
typedef struct _TDH_CONTEXT {
    ULONGLONG        ParameterValue;
    TDH_CONTEXT_TYPE ParameterType;
    ULONG            ParameterSize;
} TDH_CONTEXT, *PTDH_CONTEXT;

/*
    TRACE_EVENT_INFO
    Defines the information about the event.
*/
typedef struct _TRACE_EVENT_INFO {
    GUID                ProviderGuid;
    GUID                EventGuid;
    EVENT_DESCRIPTOR    EventDescriptor;
    DECODING_SOURCE     DecodingSource;
    ULONG               ProviderNameOffset;
    ULONG               LevelNameOffset;
    ULONG               ChannelNameOffset;
    ULONG               KeywordsNameOffset;
    ULONG               TaskNameOffset;
    ULONG               OpcodeNameOffset;
    ULONG               EventMessageOffset;
    ULONG               ProviderMessageOffset;
    ULONG               BinaryXMLOffset;
    ULONG               BinaryXMLSize;
    union {
        ULONG EventNameOffset;
        ULONG ActivityIDNameOffset;
    };
    union {
        ULONG EventAttributesOffset;
        ULONG RelatedActivityIDNameOffset;
    };
    ULONG               PropertyCount;
    ULONG               TopLevelPropertyCount;
    union {
        TEMPLATE_FLAGS Flags;
        struct {
        ULONG Reserved : 4;
        ULONG Tags : 28;
        };
    };
    EVENT_PROPERTY_INFO EventPropertyInfoArray[ANYSIZE_ARRAY];
} TRACE_EVENT_INFO, *PTRACE_EVENT_INFO;

/*
    TRACE_PROVIDER_INFO
    Defines the GUID and name for a provider.
*/
typedef struct _TRACE_PROVIDER_INFO {
    GUID  ProviderGuid;
    ULONG SchemaSource;
    ULONG ProviderNameOffset;
} TRACE_PROVIDER_INFO, *PTRACE_PROVIDER_INFO;


//====================
// Functions
//====================

/*
    EMI_MAP_FORMAT
    Macro that retrieves the event map format.

    EMI_MAP_INPUT
    Macro that retrieves the event map input.

    EMI_MAP_NAME
    Macro that retrieves the event map name.

    EMI_MAP_OUTPUT
    Macro that retrieves the event map output.

    PEI_PROVIDER_NAME
    Macro that retrieves the Provider Event Info (PEI) name.

    PFI_FIELD_MESSAGE
    Macro that retrieves the Provider Field Information (PFI) field message.

    PFI_FIELD_NAME
    Macro that retrieves the Provider Field Information (PFI) field name.

    PFI_FILTER_MESSAGE
    Macro that filters the Provider Field Information (PFI) field message.

    PFI_PROPERTY_NAME
    Macro that retrieves the Provider Field Information (PFI) property name.

    TEI_ACTIVITYID_NAME
    Macro that retrieves the Trace Event Information (TEI) activity ID name.

    TEI_CHANNEL_NAME
    Macro that retrieves the Trace Event Information (TEI) channel name.

    TEI_EVENT_MESSAGE
    Macro that retrieves the Trace Event Information (TEI) message.

    TEI_KEYWORDS_NAME
    Macro that retrieves the Trace Event Information (TEI) keywords name.

    TEI_LEVEL_NAME
    Macro that retrieves the Trace Event Information (TEI) level name.

    TEI_MAP_NAME
    Macro that retrieves the Trace Event Information (TEI) map name.

    TEI_OPCODE_NAME
    Macro that retrieves the Trace Event Information (TEI) opcode name.

    TEI_PROPERTY_NAME
    Macro that retrieves the Trace Event Information (TEI) property name.

    TEI_PROVIDER_MESSAGE
    Macro that retrieves the Trace Event Information (TEI) provider message.

    TEI_PROVIDER_NAME
    Macro that retrieves the Trace Event Information (TEI) provider name.

    TEI_RELATEDACTIVITYID_NAME
    Macro that retrieves the Trace Event Information (TEI) related activity ID name.

    TEI_TASK_NAME
    Macro that retrieves the Trace Event Information (TEI) task name.
*/

// These "Macros" are in fact utility functions implemented in Windows SDK,
// so we won't implement them.

/*
    TdhAggregatePayloadFilters
    Aggregates multiple payload filters for a single provider into a single data structure for use with the EnableTraceEx2 function.
*/
TDHAPI TdhAggregatePayloadFilters(
                ULONG                    PayloadFilterCount,
                PVOID                    *PayloadFilterPtrs,
    _In_opt_    PBOOLEAN                 EventMatchALLFlags,
    _Out_       PEVENT_FILTER_DESCRIPTOR EventFilterDescriptor
);

/*
    TdhCleanupPayloadEventFilterDescriptor
    Frees the aggregated structure of payload filters created using the TdhAggregatePayloadFilters function.
*/
TDHAPI TdhCleanupPayloadEventFilterDescriptor(
    _Inout_     PEVENT_FILTER_DESCRIPTOR EventFilterDescriptor
);

/*
    TdhCloseDecodingHandle
    Frees any resources associated with the input decoding handle.
*/
TDHAPI TdhCloseDecodingHandle(
    _In_        TDH_HANDLE Handle
);

/*
    TdhCreatePayloadFilter
    Creates a single filter for a single payload to be used with the EnableTraceEx2 function.
*/
TDHAPI TdhCreatePayloadFilter(
    _In_        LPCGUID                   ProviderGuid,
    _In_        PCEVENT_DESCRIPTOR        EventDescriptor,
    _In_        BOOLEAN                   EventMatchANY,
    _In_        ULONG                     PayloadPredicateCount,
    _In_        PPAYLOAD_FILTER_PREDICATE PayloadPredicates,
    _Out_       PVOID                     *PayloadFilter
);

/*
    TdhDeletePayloadFilter
    Frees the memory allocated for a single payload filter by the TdhCreatePayloadFilter function.
*/
TDHAPI TdhDeletePayloadFilter(
    _Inout_     PVOID *PayloadFilter
);

/*
    TdhEnumerateManifestProviderEvents
    Retrieves the list of events present in the provider manifest.
*/
TDHAPI TdhEnumerateManifestProviderEvents(
    _In_        LPGUID               ProviderGuid,
    _Out_       PPROVIDER_EVENT_INFO Buffer,
    _Inout_     ULONG                *BufferSize
);

/*
    TdhEnumerateProviderFieldInformation
    Retrieves the specified field metadata for a given provider.
*/
TDHAPI TdhEnumerateProviderFieldInformation(
    _In_        LPGUID                    pGuid,
    _In_        EVENT_FIELD_TYPE          EventFieldType,
    _Out_opt_   PPROVIDER_FIELD_INFOARRAY pBuffer,
    _Inout_     ULONG                     *pBufferSize
);

/*
    TdhEnumerateProviderFilters
    Enumerates the filters that the specified provider defined in the manifest.
*/
TDHAPI TdhEnumerateProviderFilters(
    _In_        LPGUID                Guid,
    _In_        ULONG                 TdhContextCount,
    _In_opt_    PTDH_CONTEXT          TdhContext,
    _In_        ULONG                 *FilterCount,
    _Out_opt_   PPROVIDER_FILTER_INFO *Buffer,
    _Inout_     ULONG                 *BufferSize
);

/*
    TdhEnumerateProviders
    Retrieves a list of providers that have registered a MOF class or manifest file on the computer.
*/
TDHAPI TdhEnumerateProviders(
    _Out_       PPROVIDER_ENUMERATION_INFO pBuffer,
    _Inout_     ULONG                      *pBufferSize
);

/*
    TdhEnumerateProvidersForDecodingSource
    Retrieves a list of providers that have registered a MOF class or manifest file on the computer.
*/
TDHAPI TdhEnumerateProvidersForDecodingSource(
                DECODING_SOURCE           filter,
    _Out_       PROVIDER_ENUMERATION_INFO *buffer,
    _Inout_     ULONG                     bufferSize,
    _Out_       ULONG                     *bufferRequired
);

/*
    TdhFormatProperty
    Formats a property value for display.
*/
TDHAPI TdhFormatProperty(
    _In_        PTRACE_EVENT_INFO EventInfo,
    _In_opt_    PEVENT_MAP_INFO   MapInfo,
    _In_        ULONG             PointerSize,
    _In_        USHORT            PropertyInType,
    _In_        USHORT            PropertyOutType,
    _In_        USHORT            PropertyLength,
    _In_        USHORT            UserDataLength,
    _In_        PBYTE             UserData,
    _Inout_     PULONG            BufferSize,
    _Out_opt_   PWCHAR            Buffer,
    _Out_       PUSHORT           UserDataConsumed
);

/*
    TdhGetDecodingParameter
    Retrieves the value of a decoding parameter.
*/
TDHAPI TdhGetDecodingParameter(
    _In_        TDH_HANDLE   Handle,
    _Inout_     PTDH_CONTEXT TdhContext
);

/*
    TdhGetEventInformation
    Retrieves metadata about an event.
*/
TDHAPI TdhGetEventInformation(
    _In_        PEVENT_RECORD     Event,
    _In_        ULONG             TdhContextCount,
    _In_        PTDH_CONTEXT      TdhContext,
    _Out_       PTRACE_EVENT_INFO Buffer,
    _Inout_     PULONG            BufferSize
);

/*
    TdhGetEventMapInformation
    Retrieves information about the event map contained in the event.
*/
TDHAPI TdhGetEventMapInformation(
    _In_        PEVENT_RECORD   pEvent,
    _In_        PWSTR           pMapName,
    _Out_       PEVENT_MAP_INFO pBuffer,
    _Inout_     ULONG           *pBufferSize
);

/*
    TdhGetManifestEventInformation
    Retrieves metadata about an event in a manifest.
*/
TDHAPI TdhGetManifestEventInformation(
    _In_        LPGUID            ProviderGuid,
    _In_        PEVENT_DESCRIPTOR EventDescriptor,
    _Out_       PTRACE_EVENT_INFO Buffer,
    _Inout_     ULONG             *BufferSize
);

/*
    TdhGetProperty
    Retrieves a property value from the event data.
*/
TDHAPI TdhGetProperty(
    _In_        PEVENT_RECORD             pEvent,
    _In_        ULONG                     TdhContextCount,
    _In_        PTDH_CONTEXT              pTdhContext,
    _In_        ULONG                     PropertyDataCount,
    _In_        PPROPERTY_DATA_DESCRIPTOR pPropertyData,
    _In_        ULONG                     BufferSize,
    _Out_       PBYTE                     pBuffer
);

/*
    TdhGetPropertySize
    Retrieves the size of one or more property values in the event data.
*/
TDHAPI TdhGetPropertySize(
    _In_        PEVENT_RECORD             pEvent,
    _In_        ULONG                     TdhContextCount,
    _In_        PTDH_CONTEXT              pTdhContext,
    _In_        ULONG                     PropertyDataCount,
    _In_        PPROPERTY_DATA_DESCRIPTOR pPropertyData,
    _Out_       ULONG                     *pPropertySize
);

/*
    TdhGetWppMessage
    Retrieves the formatted WPP message embedded into an EVENT_RECORD structure.
*/
TDHAPI TdhGetWppMessage(
    _In_        TDH_HANDLE    Handle,
    _In_        PEVENT_RECORD EventRecord,
    _Inout_     PULONG        BufferSize,
    _Out_       PBYTE         Buffer
);

/*
    TdhGetWppProperty
    Retrieves a specific property associated with a WPP message.
*/
TDHAPI TdhGetWppProperty(
    _In_        TDH_HANDLE    Handle,
    _In_        PEVENT_RECORD EventRecord,
    _In_        PWSTR         PropertyName,
    _Inout_     PULONG        BufferSize,
    _Out_       PBYTE         Buffer
);

/*
    TdhLoadManifest
    Loads the manifest used to decode a log file.
*/
TDHAPI TdhLoadManifest(
    _In_        PWSTR Manifest
);

/*
    TdhLoadManifestFromBinary
    Takes a NULL-terminated path to a binary file that contains metadata resources needed to decode a specific event provider.
*/
TDHAPI TdhLoadManifestFromBinary(
    _In_        PWSTR BinaryPath
);

/*
    TdhLoadManifestFromMemory
    Loads the manifest from memory.
*/
TDHAPI TdhLoadManifestFromMemory(
    _In_        LPCVOID pData,
    _In_        ULONG   cbData
);

/*
    TdhOpenDecodingHandle
    Opens a decoding handle.
*/
TDHAPI TdhOpenDecodingHandle(
    _Out_       PTDH_HANDLE Handle
);

/*
    TdhQueryProviderFieldInformation
    Retrieves information for the specified field from the event descriptions for those field values that match the given value.
*/
TDHAPI TdhQueryProviderFieldInformation(
    _In_        LPGUID                    pGuid,
    _In_        ULONGLONG                 EventFieldValue,
    _In_        EVENT_FIELD_TYPE          EventFieldType,
    _Out_       PPROVIDER_FIELD_INFOARRAY pBuffer,
    _Inout_     ULONG                     *pBufferSize
);

/*
    TdhSetDecodingParameter
    Sets the value of a decoding parameter.
*/
TDHAPI TdhSetDecodingParameter(
    _In_        TDH_HANDLE   Handle,
    _In_        PTDH_CONTEXT TdhContext
);

/*
    TdhUnloadManifest
    Unloads the manifest that was loaded by the TdhLoadManifest function.
*/
TDHAPI TdhUnloadManifest(
    _In_        PWSTR Manifest
);

/*
    TdhUnloadManifestFromMemory
    Unloads the manifest from memory.
*/
TDHAPI TdhUnloadManifestFromMemory(
    _In_        LPCVOID pData,
    _In_        ULONG   cbData
);


#ifdef __cplusplus
} // extern "C"
#endif

#endif // _WIN32_WINNT >= 0x0600

#endif // __GNUC__

#endif // __TDH_PATCH_H__