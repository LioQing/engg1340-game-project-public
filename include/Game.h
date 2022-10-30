#pragma once

#include <string>
#include <memory>
#include <type_traits>
#include <Scene.h>
#include <UI.h>
#include <Resources.h>

/**
 * @brief The main game class
 * 
 */
struct Game
{
    Game() = default;

    /**
     * @brief Construct a new Game object
     * 
     * @param is_cli Whether the game should run in CLI mode, otherwise TUI mode
     * @param exe_path The executable path
     */
    Game(bool is_cli, const std::string& exe_path);

    /**
     * @brief Run the game
     * 
     */
    void Run();

    /**
     * @brief Get the UI object
     * 
     * @return UI& A reference to the UI object
     * @throw std::runtime_error if UI is not initialized
     */
    UI& GetUI();

    /**
     * @brief Change the scene. Note the scene is not immediately changed, actual changing is happened at the start of next frame
     * 
     * @tparam T The type of the scene
     * @param scene The scene to change to
     */
    template <typename T>
    void ChangeScene(const T& scene)
    {
        static_assert(std::is_base_of<Scene, T>::value, "T must be a Scene");

        next_scene = std::make_unique<T>(scene);
    }

    /**
     * @brief Change the scene. Note the scene is not immediately changed, actual changing is happened at the start of next frame
     * 
     * @tparam TScene The type of the scene
     * @tparam TArgs The type of the arguments
     * @param args The arguments to pass to the scene constructor
     */
    template <typename TScene, typename... TArgs>
    void ChangeScene(TArgs&&... args)
    {
        static_assert(std::is_base_of<Scene, TScene>::value, "TScene must be a Scene");
        static_assert(std::is_constructible<TScene, TArgs...>::value, "TScene must be constructible with TArgs");

        next_scene = std::make_unique<TScene>(std::forward<TArgs>(args)...);
    }

    /**
     * @brief Exit the game
     * 
     */
    void Exit();

    /**
     * @brief Initialize the game resources
     * 
     * @param resources The resources to initialize
     */
    void InitResources(const Resources& resources);

    /**
     * @brief Check whether the resources are initialized
     * 
     * @return bool True if the resources are initialized, otherwise false
     */
    bool IsResourcesInitialized() const;
    
    /**
     * @brief Get the resources object
     * 
     * @return Resources& A reference to the resources object
     * @throw std::runtime_error if resources are not initialized
     */
    Resources& GetResources();

    /**
     * @brief Get the resources object
     * 
     * @return const Resources& A reference to the resources object
     * @throw std::runtime_error if resources are not initialized
     */
    const Resources& GetResources() const;

    /**
     * @brief Clear the game resources (release the memory)
     * 
     */
    void ClearResources();

    /**
     * @brief Check whether the game is in CLI or TUI mode
     * 
     */
    bool IsCLI() const;

    /**
     * @brief Get the directory of the executable
     * 
     */
    std::string GetExeDirPath() const;

private:

    bool is_running = false;
    bool is_cli = true; // if false, use tui
    std::string exe_path;
    std::unique_ptr<Scene> scene;
    std::unique_ptr<Scene> next_scene;
    std::unique_ptr<UI> ui;
    std::unique_ptr<Resources> resources;

    void Init();
    void Update();
};