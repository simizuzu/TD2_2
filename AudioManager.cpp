#include "AudioManager.h"
#include <cassert>
#include <xstring>

AudioManager* AudioManager::instance = nullptr;

AudioManager::AudioManager()
{

}


AudioManager::~AudioManager()
{
	xAudio2.Reset();

	std::list<AudioData>::iterator itr = audios.begin();

	for (; itr != audios.end(); ++itr)
	{
		itr->Unload();
	}
}

AudioManager* AudioManager::GetInstance()
{
	if (!instance)
	{
		instance = new AudioManager();
	}

	return instance;
}

void AudioManager::Destroy()
{
	MFShutdown();
	delete instance;
}

void AudioManager::Initialize()
{
	HRESULT result;

	MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);

	result = XAudio2Create(&xAudio2, 0);
	assert(SUCCEEDED(result));

#if defined(_DEBUG)
	XAUDIO2_DEBUG_CONFIGURATION debug{ 0 };
	debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
	debug.BreakMask = XAUDIO2_LOG_ERRORS;
	xAudio2->SetDebugConfiguration(&debug, 0);
#endif

	//�}�X�^�[�{�C�X�𐶐�
	result = xAudio2->CreateMasteringVoice(&masterVoice);
	assert(SUCCEEDED(result));
}

void AudioManager::Update()
{
	for (AudioData& audio : audios)
	{
		audio.playTrigger = false;
	}

	if (!playHandleArray.empty())
	{
		for (PlayAudioArray& audioArray : playHandleArray)
		{
			if (NowPlay(audioArray.handles[static_cast<const unsigned _int64>(audioArray.nowIdx)]))
			{
				audioArray.nowIdx++;

				if (static_cast<uint32_t>(audioArray.nowIdx) < audioArray.handles.size())
				{
					PlayWave(audioArray.handles[static_cast<const unsigned _int64>(audioArray.nowIdx)]);
				}
			}
		}

		//�S���Đ�����������폜
		std::vector<PlayAudioArray>::iterator itr = playHandleArray.begin();
		for (; itr != playHandleArray.end();)
		{
			if (itr->handles.size() <= static_cast<uint32_t>(itr->nowIdx))
			{
				itr = playHandleArray.erase(itr);
			}
			else
			{
				++itr;
			}
		}
	}
}

bool AudioManager::NowPlay(const uint32_t& Handle)
{
	std::list<AudioData>::iterator itr = audios.begin();

	for (size_t i = 0; i < Handle; i++)
	{
		itr++;
	}

	if (itr->pSourceVoice != nullptr)
	{
		XAUDIO2_VOICE_STATE state;

		itr->pSourceVoice->GetState(&state);

		return !(state.pCurrentBufferContext == nullptr);
	}
	else
	{
		return false;
	}
}

void AudioManager::ChangeVolume(const uint32_t& Handle, float Volume)
{
	std::list<AudioData>::iterator itr = audios.begin();

	for (size_t i = 0; i < Handle; i++)
	{
		itr++;
	}

	itr->volume = Volume;

	if (itr->pSourceVoice != nullptr)
	{
		itr->pSourceVoice->SetVolume(itr->volume);
	}
}

float AudioManager::GetVolume(const uint32_t& Handle)
{
	std::list<AudioData>::iterator itr = audios.begin();

	for (size_t i = 0; i < Handle; i++)
	{
		itr++;
	}

	return itr->volume;
}

uint32_t AudioManager::LoadAudio(std::string FileName, const float& Volume)
{
	HRESULT result = 0;

	if (!audios.empty())
	{
		uint32_t i = 0;
		for (auto itr = audios.begin(); itr != audios.end(); itr++)
		{
			if (itr->filePass == FileName)
			{
				return i;
			}
			i++;
		}
	}

	audios.emplace_back(FileName);

	std::wstring path = StringToWstring(FileName);

	result = MFCreateSourceReaderFromURL(path.c_str(), NULL, &audios.back().pMFSourceReader);

	IMFMediaType* pMFMediaType{ nullptr };
	MFCreateMediaType(&pMFMediaType);
	pMFMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	pMFMediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
	audios.back().pMFSourceReader->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, pMFMediaType);

	pMFMediaType->Release();
	pMFMediaType = nullptr;
	audios.back().pMFSourceReader->GetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, &pMFMediaType);

	MFCreateWaveFormatExFromMFMediaType(pMFMediaType, &audios.back().waveFormat, nullptr);

	while (true)
	{
		IMFSample* pMFSample{ nullptr };
		DWORD dwStreamFlags{ 0 };
		audios.back().pMFSourceReader->ReadSample(((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, nullptr, &dwStreamFlags, nullptr, &pMFSample);

		if (dwStreamFlags & MF_SOURCE_READERF_ENDOFSTREAM)
		{
			break;
		}

		IMFMediaBuffer* pMFMediaBuffer{ nullptr };
		pMFSample->ConvertToContiguousBuffer(&pMFMediaBuffer);

		BYTE* pBuffer{ nullptr };
		DWORD cbCurrentLength{ 0 };
		pMFMediaBuffer->Lock(&pBuffer, nullptr, &cbCurrentLength);

		audios.back().mediaData.resize(audios.back().mediaData.size() + cbCurrentLength);
		memcpy(audios.back().mediaData.data() + audios.back().mediaData.size() - cbCurrentLength, pBuffer, cbCurrentLength);

		pMFMediaBuffer->Unlock();

		pMFMediaBuffer->Release();
		pMFSample->Release();
	}

	uint32_t handle = static_cast<uint32_t>(audios.size() - 1);

	ChangeVolume(handle, Volume);

	return handle;
}

int32_t AudioManager::PlayWave(const uint32_t& Handle, bool LoopFlag)
{
	HRESULT result;

	std::list<AudioData>::iterator itr = audios.begin();
	for (size_t i = 0; i < Handle; i++)
	{
		itr++;
	}

	//�����ɓ����������Đ����Ȃ�
	if (itr->playTrigger)
	{
		return -1;
	}

	//���[�v�Đ��ōĐ����悤�Ƃ�������ɗ���Ă�����̂��~
	if (LoopFlag && NowPlay(Handle))
	{
		itr->pSourceVoice->Stop();
	}


	xAudio2->CreateSourceVoice(&itr->pSourceVoice, itr->waveFormat);

	XAUDIO2_BUFFER buffer{ 0 };
	buffer.pAudioData = itr->mediaData.data();
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = sizeof(BYTE) * static_cast<UINT32>(itr->mediaData.size());

	if (LoopFlag)
	{
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	//���ʂ̐ݒ�
	itr->pSourceVoice->SetVolume(itr->volume);

	//�g�`�f�[�^�̍Đ�
	result = itr->pSourceVoice->SubmitSourceBuffer(&buffer);
	result = itr->pSourceVoice->Start();

	itr->playTrigger = true;

	return 1;
}

int32_t AudioManager::PlayWaveArray(const std::vector<uint32_t>& handles)
{
	if (handles.empty())
	{
		return 0;
	}

	playHandleArray.emplace_back(handles);
	return PlayWave(handles[0]);
};

void AudioManager::StopWave(const uint32_t& Handle)
{
	std::list<AudioData>::iterator itr = audios.begin();

	for (size_t i = 0; i < Handle; i++)
	{
		itr++;
	}

	if (itr->pSourceVoice != nullptr)
	{
		itr->pSourceVoice->Stop();
		itr->pSourceVoice->FlushSourceBuffers();
	}
}

std::wstring AudioManager::StringToWstring(const std::string& string)
{
	auto const dest_size = ::MultiByteToWideChar(CP_ACP, 0U, string.data(), -1, nullptr, 0U);
	std::vector<wchar_t> dest(static_cast<const unsigned _int64>(dest_size), L'\0');
	if (::MultiByteToWideChar(CP_ACP, 0U, string.data(), -1, dest.data(), static_cast<int>(dest.size())) == 0)
	{
		throw std::system_error{ static_cast<int>(::GetLastError()), std::system_category() };
	}
	dest.resize(std::char_traits<wchar_t>::length(dest.data()));
	dest.shrink_to_fit();
	return std::wstring(dest.begin(), dest.end());
}

AudioData::AudioData(std::string FilePass) :filePass(FilePass)
{
}

void AudioData::Unload()
{
	if (pBuffer != nullptr)
	{
		delete[] pBuffer;
	}
}

PlayAudioArray::PlayAudioArray(const std::vector<uint32_t>& Handles) :handles(Handles)
{
}
