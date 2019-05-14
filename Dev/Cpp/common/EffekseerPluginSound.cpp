#include <algorithm>
#include "EffekseerPluginSound.h"

namespace EffekseerPlugin
{
	void* SoundLoader::Load( const EFK_CHAR* path ){
		// ���\�[�X�e�[�u�����������đ��݂����炻����g��
		auto it = resources.find((const char16_t*)path);
		if (it != resources.end()) {
			it->second.referenceCount++;
			return (void*)it->second.soundID;
		}

		// Unity�Ń��[�h
		SoundResource res;
		res.soundID = load( (const char16_t*)path );
		if (res.soundID == 0) {
			return 0;
		}
		
		// ���\�[�X�e�[�u���ɒǉ�
		resources.insert( std::make_pair((const char16_t*)path, res ) );
		
		return (void*)res.soundID;
	}

	void SoundLoader::Unload( void* source ){
		if (source == nullptr) {
			return;
		}
		uintptr_t soundID = (uintptr_t)source;

		// �A�����[�h���郂�f��������
		auto it = std::find_if(resources.begin(), resources.end(), 
			[soundID](const std::pair<std::u16string, SoundResource>& pair){
				return pair.second.soundID == soundID;
			});

		// �Q�ƃJ�E���^��0�ɂȂ�������ۂɃA�����[�h
		it->second.referenceCount--;
		if (it->second.referenceCount <= 0) {
			unload( it->first.c_str() );
			resources.erase(it);
		}
	}
	
	SoundLoader* SoundLoader::Create(
		SoundLoaderLoad load,
		SoundLoaderUnload unload)
	{
		return new SoundLoader(load, unload);
	}

	SoundPlayer* SoundPlayer::Create(
		SoundPlayerPlay play,
		SoundPlayerStopTag stopTag,
		SoundPlayerPauseTag pauseTag,
		SoundPlayerCheckPlayingTag checkPlayingTag,
		SoundPlayerStopAll stopAll)
	{
		return new SoundPlayer(play, stopTag, pauseTag, checkPlayingTag, stopAll);
	}
}