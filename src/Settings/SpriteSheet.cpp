#include "SpriteSheet.hpp"

#include "Core/Texture.hpp"
#include "Core/TextureUtils.hpp"
#include "Util/Logger.hpp"
#include "Util/Time.hpp"

#include "Util/TransformUtils.hpp"

#include "config.hpp"

SpriteSheet::SpriteSheet(const std::string filepath, glm::vec2 frameSize,
                         const std::vector<std::size_t> &frames, bool play,
                         std::size_t interval, bool looping,
                         std::size_t cooldown)
    : m_Path(filepath),
      m_FrameSize(frameSize),
      m_Frames(frames),
      m_State(play ? State::PLAY : State::PAUSE),
      m_Interval(interval),
      m_Looping(looping),
      m_Cooldown(cooldown),
      m_Index(0) {
    if (s_Program == nullptr) {
        InitProgram();
    }
    if (s_VertexArray == nullptr) {
        InitVertexArray();
    }
    if (s_UniformBuffer == nullptr) {
        InitUniformBuffer();
    }

    m_Surface =
        std::unique_ptr<SDL_Surface, std::function<void(SDL_Surface *)>>{
            IMG_Load(filepath.c_str()),
            SDL_FreeSurface,
        };
    if (m_Surface == nullptr) {
        LOG_ERROR("Failed to load image: '{}'", filepath);
        LOG_ERROR("{}", IMG_GetError());
    }

    m_Texture = std::make_unique<Core::Texture>(
        Core::SdlFormatToGlFormat(m_Surface->format->format), m_Surface->w,
        m_Surface->h, m_Surface->pixels);
    m_Size = {m_Surface->w, m_Surface->h};
    m_col = static_cast<int>(m_Surface->w / m_FrameSize.x);
}

void SpriteSheet::SetCurrentFrame(std::size_t index) {
    m_Index = index;
    if (m_State == State::ENDED || m_State == State::COOLDOWN) {
        /*this make sure if user setframe on ENDED/COOLDOWN, will play from
         * where you set the frame*/
        m_IsChangeFrame = true;
    }
}

void SpriteSheet::Draw(const Util::Transform &transform, const float zIndex) {
    SDL_Rect displayRect{
        static_cast<int>(m_FrameSize.x * (m_Frames[m_Index] % m_col)),
        static_cast<int>(m_FrameSize.y * (m_Frames[m_Index] / m_col)),
        static_cast<int>(m_FrameSize.x), static_cast<int>(m_FrameSize.y)};
    SetDrawRect(displayRect);
    auto data = Util::ConvertToUniformBufferData(transform, m_Size, zIndex);
    s_UniformBuffer->SetData(0, data);

    m_Texture->Bind(UNIFORM_SURFACE_LOCATION);
    s_Program->Bind();
    s_Program->Validate();

    s_VertexArray->Bind();
    s_VertexArray->DrawTriangles();
    Update();
}

void SpriteSheet::Play() {
    if (m_State == State::PLAY)
        return;
    if (m_State == State::ENDED || m_State == State::COOLDOWN) {
        m_Index = m_IsChangeFrame ? m_Index : 0;
        m_IsChangeFrame = false;
    }
    m_State = State::PLAY;
}

void SpriteSheet::Pause() {
    if (m_State == State::PLAY || m_State == State::COOLDOWN) {
        m_State = State::PAUSE;
    }
}

void SpriteSheet::Update() {
    unsigned long nowTime = Util::Time::GetElapsedTimeMs();
    if (m_State == State::PAUSE || m_State == State::ENDED) {
        LOG_TRACE("[ANI] is pause");
        return;
    }

    if (m_State == State::COOLDOWN) {
        if (nowTime >= m_CooldownEndTime) {
            Play();
        }
        return;
    }

    m_TimeBetweenFrameUpdate += Util::Time::GetDeltaTime();
    unsigned int updateFrameCount =
        static_cast<int>(m_TimeBetweenFrameUpdate / (m_Interval / 1000.0));

    if (updateFrameCount <= 0)
        return;

    m_Index += updateFrameCount;
    m_TimeBetweenFrameUpdate = 0;

    unsigned int const totalFramesCount = m_Frames.size();
    if (m_Index >= totalFramesCount) {
        if (m_Looping) {
            m_CooldownEndTime = nowTime + m_Cooldown;
        }
        m_State = m_Looping ? State::COOLDOWN : State::ENDED;
        m_Index = m_Frames.size() - 1;
    }
}

void SpriteSheet::InitProgram() {
    // TODO: Create `BaseProgram` from `Program` and pass it into `Drawable`
    s_Program =
        std::make_unique<Core::Program>(PTSD_DIR "/assets/shaders/Base.vert",
                                        PTSD_DIR "/assets/shaders/Base.frag");
    s_Program->Bind();

    GLint location = glGetUniformLocation(s_Program->GetId(), "surface");
    glUniform1i(location, UNIFORM_SURFACE_LOCATION);
}

void SpriteSheet::InitVertexArray() {
    s_VertexArray = std::make_unique<Core::VertexArray>();

    // NOLINTBEGIN
    // These are vertex data for the rectangle but clang-tidy has magic
    // number warnings

    // Vertex
    s_VertexArray->AddVertexBuffer(std::make_unique<Core::VertexBuffer>(
        std::vector<float>{
            -0.5F, 0.5F,  //
            -0.5F, -0.5F, //
            0.5F, -0.5F,  //
            0.5F, 0.5F,   //
        },
        2));

    // UV
    s_VertexArray->AddVertexBuffer(std::make_unique<Core::VertexBuffer>(
        std::vector<float>{
            0.0F, 0.0F, //
            0.0F, 1.0F, //
            1.0F, 1.0F, //
            1.0F, 0.0F, //
        },
        2));

    // Index
    s_VertexArray->SetIndexBuffer(
        std::make_unique<Core::IndexBuffer>(std::vector<unsigned int>{
            0, 1, 2, //
            0, 2, 3, //
        }));
    // NOLINTEND
}

void SpriteSheet::InitUniformBuffer() {
    s_UniformBuffer = std::make_unique<Core::UniformBuffer<Core::Matrices>>(
        *s_Program, "Matrices", 0);
}

void SpriteSheet::SetDrawRect(const SDL_Rect displayRect) {
    // can't just simplely use SDL_SetClipRect, because we use opengl to render
    // and I'm not sure this way if cost a lot performance?
    if (displayRect.h + displayRect.y > m_Surface->h ||
        displayRect.w + displayRect.x > m_Surface->w) {
        LOG_DEBUG("SetDrawRect OverRange");
        return;
    }

    auto targetSurface =
        std::unique_ptr<SDL_Surface, std::function<void(SDL_Surface *)>>{
            SDL_CreateRGBSurfaceWithFormat(0, displayRect.w, displayRect.h, 32,
                                           m_Surface->format->format),
            SDL_FreeSurface,
        };

    int isCopyWork = SDL_BlitSurface(m_Surface.get(), &displayRect,
                                     targetSurface.get(), NULL);
    if (isCopyWork != 0) {
        LOG_ERROR("{}", SDL_GetError());
        return;
    }

    m_Texture->UpdateData(
        Core::SdlFormatToGlFormat(targetSurface->format->format),
        targetSurface->w, targetSurface->h, targetSurface->pixels);
    m_Size = {targetSurface->w, targetSurface->h};
}

std::unique_ptr<Core::Program> SpriteSheet::s_Program = nullptr;
std::unique_ptr<Core::VertexArray> SpriteSheet::s_VertexArray = nullptr;
std::unique_ptr<Core::UniformBuffer<Core::Matrices>>
    SpriteSheet::s_UniformBuffer = nullptr;
