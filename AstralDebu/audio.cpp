#include "audio.h"
using namespace audioNS;

Audio::Audio(){
	xact = NULL;
	wave = NULL;
	sound = NULL;
	cueIndex = 0;
	map = NULL;
	data = NULL;
	global = NULL;
	 
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (SUCCEEDED(hr)) initialized = true;
	else initialized = false;
}


Audio::~Audio(){
	if (xact){
		xact->ShutDown();
		xact->Release();
	}
	
	if (data) delete[] data;
	data = NULL;
	
	if (map) UnmapViewOfFile(map);
	map = NULL;

	if (initialized) CoUninitialize();
}

HRESULT Audio::initialize(){
	HRESULT result = E_FAIL;
	HANDLE hFile;
	DWORD fileSize;
	DWORD bytesRead;
	DWORD globalSize;
	HANDLE hMapFile;

	if (!initialized) return result;
	
	result = XACT3CreateEngine(0, &xact);
	if (FAILED(result) || xact == NULL) return E_FAIL;

	hFile = CreateFile((WAV_FILE_DIR + "\\" + WAV_FILE_GLOBAL).c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile)
	{
		//ファイルサイズを取得
		globalSize = GetFileSize(hFile, NULL);

		//ファイルサイズ取得判定
		if (globalSize != INVALID_FILE_SIZE)
		{
			global = CoTaskMemAlloc(globalSize);
			if (global)
			{
				DWORD byteRead;
				//ファイルを読み込む
				ReadFile(hFile, global, globalSize, &byteRead, NULL);
			}
		}
		//ファイルハンドルを閉じる
		CloseHandle(hFile);
	}

	XACT_RUNTIME_PARAMETERS param = {0};
	param.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;
	param.pGlobalSettingsBuffer = global;
	param.globalSettingsBufferSize = globalSize;
	param.globalSettingsFlags = XACT_FLAG_GLOBAL_SETTINGS_MANAGEDATA;
	result = xact->Initialize(&param);
	if (FAILED(result)) return result;

	result = E_FAIL;
	hFile = CreateFile((WAV_FILE_DIR + "\\" + WAV_FILE_WAVE).c_str(), GENERIC_READ,
		FILE_SHARE_READ, NULL, OPEN_EXISTING
		, 0, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		fileSize = GetFileSize(hFile,NULL);
		if (fileSize != -1) {
			hMapFile = CreateFileMapping(hFile, NULL,
				PAGE_READONLY, 0,
				fileSize, NULL);
			if (hMapFile){
				map = MapViewOfFile(hMapFile, FILE_MAP_READ
					, 0, 0, 0);
				if (map)
					result = xact->CreateInMemoryWaveBank(map, fileSize, 0, 0, &wave);
				CloseHandle(hMapFile);
			}
		}
		CloseHandle(hFile);
	}
	if (FAILED(result)) return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);

	result = E_FAIL;
	hFile = CreateFile((WAV_FILE_DIR + "\\" + WAV_FILE_SOUND).c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if( hFile != INVALID_HANDLE_VALUE )
    {
        fileSize = GetFileSize( hFile, NULL );
        if( fileSize != -1 )
        {
            data = new BYTE[fileSize];
            if( data )
            {
                if( 0 != ReadFile( hFile, data, fileSize, &bytesRead, NULL ) )
                    result = xact->CreateSoundBank( data, fileSize, 0, 0, &sound );
            }
        }
        CloseHandle( hFile );
    }
    if( FAILED( result ) ) return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );


    return S_OK;
}

void Audio::run(){
	if (xact == NULL) return;
	xact->DoWork();
}

void Audio::playCue(const char cue[]){
	if (sound == NULL) return;
	cueIndex = sound->GetCueIndex(cue);
	sound->Play(cueIndex,0,0,NULL);
}

void Audio::stopCue(const char cue[]){
	if (sound == NULL) return;
	cueIndex = sound->GetCueIndex(cue);
	sound->Stop(cueIndex, XACT_FLAG_SOUNDBANK_STOP_IMMEDIATE);
}

bool Audio::isPlaying(const char cue[]){
	XACT_CUE_PROPERTIES prop;
	if (sound == NULL) return false;
	cueIndex = sound->GetCueIndex(cue);

	sound->GetCueProperties(cueIndex,&prop);
	return (prop.currentInstances>0);
}

void Audio::setVolumeBgm(double volume){
	if (sound == NULL) return;
	category = xact->GetCategory(CT_BGM);
	xact->SetVolume(category, (float)volume);
}

void Audio::setVolumeSound(double volume){
	if (sound == NULL) return;
	category = xact->GetCategory(CT_DEFAULT);
	xact->SetVolume(category, (float)volume);
}