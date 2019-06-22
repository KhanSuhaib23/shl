// TODO(Suhaib): No error checking right now
// TODO(Suhaib): Only shared mode
// TODO(Suhaib): Try to remove the malloc for WAVEFORMATEX


#ifndef SHL_AUDIO_H
#define SHL_AUDIO_H

#if !defined(SHL_PLATFORM_DETECTED)
#define SHL_PLATFORM_DETECTED

#if defined(_WIN32)

#if defined(_WIN64)
#define SHL_WIN32
#define SHL_WIN
#else 
#define SHL_WIN64
#define SHL_WIN
#endif 

#endif

#if defined(__cplusplus)
#define SHL_IS_CPP
#else
#define SHL_IS_C
#endif

#endif


#if defined(SHL_WIN)
#define INITGUID
#define COBJMACROS
#define CONST_VTABLE
#define NONEWWAVE

#include <windows.h>
#include <windowsx.h>
#include <xinput.h>
#include <Mmdeviceapi.h>
#include <Audioclient.h>
#include <Audiopolicy.h>
#include <malloc.h>


#endif

#if defined(SHL_IS_CPP)
#if defined(SHL_FILE_STATIC)
#define SHLAPI static extern "C"
#else
#define SHLAPI extern "C" 
#endif
#else
#if defined(SHL_FILE_STATIC)
#define SHLAPI static
#else
#define SHLAPI
#endif
#endif



typedef enum shl_audio_data_type
{
    SHL_AUDIO_INT8,
    SHL_AUDIO_INT16,
    SHL_AUDIO_FLOAT32,
    
} shl_audio_data_type;

typedef struct shl_audio
{
    
    float buffer_size_seconds;
    int samples_per_second;
    int buffer_size_samples;
    shl_audio_data_type data_type;
    int periodicity;
    int min_samples_to_write;
    int max_samples_to_write;
    
    int actual_samples_written;
    void* buffer;
    
#if defined(SHL_WIN)
    
    IMMDeviceEnumerator* device_enumerator;
    IAudioClient* audio_client;
    IAudioRenderClient* audio_render_client;
    
    WAVEFORMATEX* wave_format;
    
#endif
    
} shl_audio;

SHLAPI void shl_audio_init();
SHLAPI void shl_audio_create(shl_audio* audio);
SHLAPI void shl_audio_pull(shl_audio* audio);
SHLAPI void shl_audio_push(shl_audio* audio);

#endif

#ifdef SHL_AUDIO_IMPLEMENTATION
#if defined(SHL_WIN)

#if defined(SHL_IS_CPP)
#define SHL__REF(val) val
#else
#define SHL__REF(val) &##val
#endif

#define SHL__CoCreateInstance(clsid, unknown, context, iid, interface_pointer) CoCreateInstance(SHL__REF(clsid), (unknown), (context), SHL__REF(iid), (interface_pointer))

#if defined(SHL_IS_CPP)
#define SHL__GetDefaultAudioEndpoint(object, data_flow, role, endpoint) (object)->GetDefaultAudioEndpoint((data_flow), (role), (endpoint))
#define SHL__Activate(object, iid, exec_context, activation_params, interface) (object)->Activate(SHL__REF(iid), (exec_context), (activation_params), (interface))
#define SHL__GetDevicePeriod(object, def_latency, min_latency) (object)->GetDevicePeriod((def_latency), (min_latency))
#define SHL__GetMixFormat(object, wave_format) (object)->GetMixFormat((wave_format))
#define SHL__Initialize(object, stream_mode, stream_flags, buffer_duration, latency, wave_format, session_guid) (object)->Initialize((stream_mode), (stream_flags), (buffer_duration), (latency), (wave_format), (session_guid))
#define SHL__GetService(object, iid, service) (object)->GetService(SHL__REF(iid), (service))
#define SHL__Start(object) (object)->Start()
#define SHL__GetCurrentPadding(object, padding) (object)->GetCurrentPadding((padding))
#define SHL__GetRenderBuffer(object, frames_requested, data) (object)->GetBuffer((frames_requested), (data))
#define SHL__ReleaseRenderBuffer(object, frames_written, flags) (object)->ReleaseBuffer((frames_written), (flags))


#else
#define SHL__GetDefaultAudioEndpoint(object, data_flow, role, endpoint) IMMDeviceEnumerator_GetDefaultAudioEndpoint((object), (data_flow), (role), (endpoint));
#define SHL__Activate(object, iid, exec_context, activation_params, interface) IMMDevice_Activate((object), SHL__REF(iid), (exec_context), (activation_params), (interface))
#define SHL__GetDevicePeriod(object, def_latency, min_latency) IAudioClient_GetDevicePeriod((object), (def_latency), (min_latency))
#define SHL__GetMixFormat(object, wave_format) IAudioClient_GetMixFormat((object), (wave_format))
#define SHL__Initialize(object, stream_mode, stream_flags, buffer_duration, latency, wave_format, session_guid) IAudioClient_Initialize((object), (stream_mode), (stream_flags), (buffer_duration), (latency), (wave_format), (session_guid))
#define SHL__GetService(object, iid, service) IAudioClient_GetService((object), SHL__REF(iid), (service))
#define SHL__Start(object) IAudioClient_Start((object))
#define SHL__GetCurrentPadding(object, padding) IAudioClient_GetCurrentPadding((object), (padding))
#define SHL__GetRenderBuffer(object, frames_requested, data) IAudioRenderClient_GetBuffer((object), (frames_requested), (data))
#define SHL__ReleaseRenderBuffer(object, frames_written, flags) IAudioRenderClient_ReleaseBuffer((object), (frames_written), (flags))
#endif

static const CLSID clsid_device_enumerator = 
{
    0xbcde0395, 0xe52f, 0x467c,
    {0x8e, 0x3d, 0xc4, 0x57, 0x92, 0x91, 0x69, 0x2e}
};

static const IID iid_device_enumerator = 
{
    0xa95664d2, 0x9614, 0x4f35,
    {0xa7, 0x46, 0xde, 0x8d, 0xb6, 0x36, 0x17, 0xe6}
};

static const IID iid_audio_client = 
{
    0x1cb9ad4c, 0xdbfa, 0x4c32,
    {0xb1, 0x78, 0xc2, 0xf5, 0x68, 0xa7, 0x03, 0xb2}
};

static const IID iid_audio_render_client = 
{
    0xf294acfc, 0x3146, 0x4483,
    {0xa7, 0xbf, 0xad, 0xdc, 0xa7, 0xc2, 0x60, 0xe2}
};

static const IID iid_audio_clock = 
{
    0xcd63314f, 0x3fba, 0x4a1b,
    {0x81, 0x2c, 0xef, 0x96, 0x35, 0x87, 0x28, 0xe7}
};


SHLAPI void shl_audio_init()
{
    CoInitializeEx(0, COINIT_APARTMENTTHREADED);
}

SHLAPI void shl_audio_create(shl_audio* audio)
{
    
    
    
    HRESULT hr;
    
    hr = SHL__CoCreateInstance(clsid_device_enumerator, NULL, CLSCTX_ALL, iid_device_enumerator, (void**) (&audio->device_enumerator));
    
    IMMDevice *sound_op_device_handle;
    hr = SHL__GetDefaultAudioEndpoint(audio->device_enumerator, eRender, eConsole, &sound_op_device_handle);
    
    hr = SHL__Activate(sound_op_device_handle, iid_audio_client, CLSCTX_ALL, 0, (void**)(&audio->audio_client));
    
    REFERENCE_TIME minimum_latency_ws, default_latency_ws;
    
    hr = SHL__GetDevicePeriod(audio->audio_client, &default_latency_ws, &minimum_latency_ws);
    
    //hr = IAudioClient_GetStreamLatency()
    
    audio->periodicity = (int) (default_latency_ws / 10000);
    
    audio->wave_format = (WAVEFORMATEX*) malloc(sizeof(WAVEFORMATEX));
    
    hr = SHL__GetMixFormat(audio->audio_client, &audio->wave_format);
    
    audio->samples_per_second = audio->wave_format->nSamplesPerSec;
    
    audio->buffer_size_samples = (int) (audio->buffer_size_seconds * audio->samples_per_second + 0.5);
    
    hr = SHL__Initialize(audio->audio_client, 
                         AUDCLNT_SHAREMODE_SHARED,
                         0, 
                         (REFERENCE_TIME) (audio->buffer_size_seconds * 10000000), 
                         0, 
                         audio->wave_format, 
                         0);
    
    
    hr = SHL__GetService(audio->audio_client, iid_audio_render_client, (void**) (&audio->audio_render_client));
    
    
    
    int bps = audio->wave_format->wBitsPerSample;
    
    if (bps == 8)       audio->data_type = SHL_AUDIO_INT8;
    else if (bps == 16) audio->data_type = SHL_AUDIO_INT16;
    else if (bps == 32) audio->data_type = SHL_AUDIO_FLOAT32;
    
    SHL__Start(audio->audio_client);
    
}

SHLAPI void shl_audio_pull(shl_audio* audio)
{
    UINT32 frames_padding;
    
    SHL__GetCurrentPadding(audio->audio_client, &frames_padding);
    
    audio->max_samples_to_write = (int) audio->buffer_size_samples - frames_padding;
    audio->min_samples_to_write = (int) (audio->periodicity * audio->samples_per_second / 1000);
    
    audio->min_samples_to_write = (audio->max_samples_to_write > audio->min_samples_to_write ? audio->min_samples_to_write : audio->max_samples_to_write);
    
    
    SHL__GetRenderBuffer(audio->audio_render_client, audio->max_samples_to_write, (BYTE**) &audio->buffer);
    
}

SHLAPI void shl_audio_push(shl_audio* audio)
{
    SHL__ReleaseRenderBuffer(audio->audio_render_client, audio->actual_samples_written, 0);
}

#undef SHL__REF



#endif
#endif