#include <Game.h>

#include <thread>
#include <stdexcept>
#include <MainMenu.h>
#include <InMap.h>
#include <CLI.h>
#include <TUI.h>
#include <SaveManager.h>

Game::Game(bool is_cli, const std::string& exe_path) : is_cli(is_cli), exe_path(exe_path)
{
}

void Game::Run()
{
    Init();

    while (is_running)
    {
        Update();
    }

    scene->Finish(*this);
}

UI& Game::GetUI()
{
    if (!ui)
        throw std::runtime_error("UI is not initialized");

    return *ui;
}

void Game::Init()
{
    SaveManager::Init(exe_path);

    is_running = true;
    scene = std::make_unique<MainMenu>();

    if (is_cli)
    {
        ui = std::make_unique<CLI>();
    }
    else
    {
        ui = std::make_unique<TUI>();
    }

    scene->Start(*this);
}

void Game::Update()
{
    // change scene if available
    if (next_scene)
    {
        scene->Finish(*this);
        scene = std::move(next_scene);
        scene->Start(*this);
    }

    // update
    ui->PreUpdate(*this);
    scene->Update(*this);
    ui->PostUpdate(*this);
}

void Game::Exit()
{
    is_running = false;
}

void Game::InitResources(const Resources& resources)
{
    this->resources = std::make_unique<Resources>(resources);
}

bool Game::IsResourcesInitialized() const
{
    return resources != nullptr;
}

Resources& Game::GetResources()
{
    if (!resources)
        throw std::runtime_error("Resources are not initialized");
    
    return *resources;
}

const Resources& Game::GetResources() const
{
    if (!resources)
        throw std::runtime_error("Resources are not initialized");
    
    return *resources;
}

void Game::ClearResources()
{
    resources.release();
}

bool Game::IsCLI() const
{
    return is_cli;
}

std::string Game::GetExeDirPath() const
{
    return exe_path.substr(0, exe_path.find_last_of("/\\"));
}