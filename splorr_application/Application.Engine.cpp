#include "Application.Engine.h"
#include "Application.Platform.h"
#include <Data.JSON.h>
#include <memory>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
namespace application
{
	static const std::string WIDTH = "width";
	static const std::string HEIGHT = "height";
	static const std::string LOGICAL_WIDTH = "logicalWidth";
	static const std::string LOGICAL_HEIGHT = "logicalHeight";
	static const std::string TITLE = "title";
	static const std::string ICON = "icon";
	static const std::string MIXER_FREQUENCY = "mixerFrequency";
	static const std::string CHANNEL_COUNT = "channelCount";
	static const std::string CHUNK_SIZE = "chunkSize";

	static std::shared_ptr<SDL_Window> window = nullptr;
	static std::shared_ptr<Engine::Renderer> renderer = nullptr;

	void Engine::SetFullscreen(bool flag)
	{
		SDL_SetWindowFullscreen(window.get(), (flag) ? (SDL_WINDOW_FULLSCREEN_DESKTOP) : (0));
	}

	bool Engine::IsFullscreen()
	{
		return (SDL_GetWindowFlags(window.get()) & SDL_WINDOW_FULLSCREEN) > 0;
	}

	extern void Start(const std::vector<std::string>&);
	extern bool IsRunning();
	extern void Updatify(unsigned int);
	extern void HandleEvent(const SDL_Event&);

	static std::map<int, SDL_GameController*> controllers;

	static void FinishControllers()
	{
		for (auto controller : controllers)
		{
			SDL_GameControllerClose(controller.second);
		}
		controllers.clear();
	}

	static void HookMixQuit()//TODO: to audio platform
	{
		Mix_Quit();
	}

	static void StartAudio(const nlohmann::json& properties)
	{
		int mixerFrequency = properties[MIXER_FREQUENCY];
		int channelCount = properties[CHANNEL_COUNT];
		int chunkSize = properties[CHUNK_SIZE];
		Mix_Init(MIX_INIT_OGG);//TODO: to audio platform
		Mix_OpenAudio
		(
			mixerFrequency,
			MIX_DEFAULT_FORMAT,
			channelCount,
			chunkSize
		);
		atexit(HookMixQuit);
		atexit(Mix_CloseAudio);
	}

	static void StartWindow(const nlohmann::json& properties)
	{
		int windowWidth = properties[WIDTH];
		int windowHeight = properties[HEIGHT];
		int logicalWidth = properties[LOGICAL_WIDTH];
		int logicalHeight = properties[LOGICAL_HEIGHT];
		std::string windowTitle = properties[TITLE];
		std::string iconFileName = properties[ICON];
		SDL_Window* pw = nullptr;//TODO: to application platform
		SDL_Renderer* pr = nullptr;
		SDL_CreateWindowAndRenderer(
			windowWidth,
			windowHeight,
			SDL_WINDOW_RESIZABLE,
			&pw,
			&pr);
		window = std::shared_ptr<SDL_Window>(pw, SDL_DestroyWindow);
		renderer = std::make_shared<Engine::Renderer>(pr);
		SDL_RenderSetLogicalSize(renderer.get()->renderer.get(), logicalWidth, logicalHeight);
		SDL_SetWindowTitle(window.get(), windowTitle.c_str());
		auto iconSurface = IMG_Load(iconFileName.c_str());
		SDL_SetWindowIcon(window.get(), iconSurface);
		SDL_FreeSurface(iconSurface);
	}

	static void StartControllers()
	{
		atexit(FinishControllers);
		for (int index = 0; index < SDL_NumJoysticks(); ++index)
		{
			if (SDL_IsGameController(index))
			{
				controllers[index] = SDL_GameControllerOpen(index);
			}
		}
	}

	static void DoStart(const std::string& configFile, const std::vector<std::string>& arguments)
	{
		Platform::Initialize();

		auto properties = ::data::JSON::Load(configFile);
		StartAudio(properties);
		StartWindow(properties);
		StartControllers();
		Start(arguments);
	}

	static void DoPump()
	{
		SDL_Event evt;
		auto currentTicks = SDL_GetTicks();
		while (IsRunning())
		{
			auto frameTicks = SDL_GetTicks();
			Updatify(frameTicks - currentTicks);
			currentTicks = frameTicks;
			Engine::Render(renderer);
			SDL_RenderPresent(renderer.get()->renderer.get());
			while (SDL_PollEvent(&evt))
			{
				HandleEvent(evt);
			}
		}
	}

	static void DoFinish()
	{
		Mix_CloseAudio();
		Mix_Quit();
	}

	int Engine::Run(const std::vector<std::string>& arguments)
	{
		atexit(DoFinish);
		DoStart(CONFIG_FILE, arguments);
		DoPump();
		return 0;
	}

	Engine::Renderer::Renderer(SDL_Renderer* r)
	{
		this->renderer = std::shared_ptr<SDL_Renderer>(r, SDL_DestroyRenderer);
	}
	void Engine::Renderer::Copy(std::shared_ptr<SDL_Texture> texture, const SDL_Rect* source, const SDL_Rect* destination, double angle) const
	{
		SDL_RenderCopyEx(renderer.get(), texture.get(), source, destination, angle, nullptr, SDL_FLIP_NONE);
	}
}
