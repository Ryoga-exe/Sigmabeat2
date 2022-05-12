#include "Game.hpp"

Game::Game(const InitData& init)
    : IScene(init)
    , m_fieldRT(FieldSize)
    , VS(HLSL{ U"example/shader/hlsl/homography.hlsl", U"VS" } | GLSL{ U"example/shader/glsl/homography.vert", {{ U"VSConstants2D", 0 }, { U"VSHomography", 1} } })
    , PS(HLSL{ U"example/shader/hlsl/homography.hlsl", U"PS" } | GLSL{ U"example/shader/glsl/homography.frag", {{ U"PSConstants2D", 0 }, { U"PSHomography", 1} } }) {

    Scene::SetBackground(Palette::Whitesmoke);
    Scene::SetLetterbox(Palette::Whitesmoke);

    m_speed = 1.00;
    m_combo = 0;
    m_judgementYPos = DefaultJudmentYPos;
    m_laneWidth = (FieldSize.x - 2 * EdgeWidth) / LaneNum;

    if (not (VS && PS)) {
        throw Error{ U"Failed to load shader files" };
    }

}

void Game::update() {

}

void Game::draw() const {
    drawField();
}

void Game::drawField() const {

    auto [SceneW, SceneH] = Scene::Size();

    auto TopW = SceneW * 0.20;
    auto BottomW = SceneW * 0.70;

    Quad quad{ Vec2{(SceneW - BottomW) / 2, SceneH}, Vec2{(SceneW + BottomW) / 2, SceneH}, Vec2{(SceneW + TopW) / 2, 0}, Vec2{(SceneW - TopW) / 2, 0} };

    {
        const ScopedRenderTarget2D target(m_fieldRT.clear(UI::BackgroundColor));


        for (uint8 i = 1; i < LaneNum; i++) {
            RectF{ Arg::topCenter(EdgeWidth + m_laneWidth * i, 0.0), 2.0, (double)FieldSize.y }.draw(UI::LaneLineColor);
        }


        drawJudmentLine();

        // debug
        drawNote(Note{ .lane = 0 });


        Rect{ Arg::topRight(FieldSize.x, 0), EdgeWidth, FieldSize.y }.draw(UI::EdgeColor);
        Rect{ Arg::topLeft(0, 0), EdgeWidth, FieldSize.y }.draw(UI::EdgeColor);

        Rect{ FieldSize }.draw(Arg::top = ColorF(UI::BlurColor, 0), Arg::bottom = ColorF(UI::BlurColor, 0.8));

    }

    /*--------------------------*/

    Graphics2D::Flush();
    m_fieldRT.resolve();

    ConstantBuffer<Homography> vsHomography;
    ConstantBuffer<Homography> psHomography;

    {
        const ScopedCustomShader2D shader{ VS, PS };
        const ScopedRenderStates2D sampler{ SamplerState::ClampAniso };
        {
            const Mat3x3 mat = Mat3x3::Homography(quad);
            vsHomography = { Float4{ mat._11_12_13, 0 }, Float4{ mat._21_22_23, 0 }, Float4{ mat._31_32_33, 0 } };
            Graphics2D::SetVSConstantBuffer(1, vsHomography);

            const Mat3x3 inv = mat.inverse();
            psHomography = { Float4{ inv._11_12_13, 0 }, Float4{ inv._21_22_23, 0 }, Float4{ inv._31_32_33, 0 } };
            Graphics2D::SetPSConstantBuffer(1, psHomography);

            Rect{ 1 }(m_fieldRT).draw();
        }
    }


    // debug
    RectF{ FieldSize * 0.65 }(m_fieldRT.flipped()).draw();


}

void Game::drawJudmentLine() const {
    
    RectF{ Arg::leftCenter(0, m_judgementYPos), static_cast<double>(FieldSize.x), 7 }.drawShadow({ 0.0, 0.0 }, 12.0, 0.0, ColorF(UI::JudmentLineBlurColor));
    
    RectF{ 0, m_judgementYPos, static_cast<double>(FieldSize.x), -3 }.draw(Arg::top(UI::JudmentLineColor), Arg::bottom(ColorF(UI::JudmentLineBlurColor, 0.5)));
    RectF{ 0, m_judgementYPos, static_cast<double>(FieldSize.x), 3 }.draw(Arg::top(UI::JudmentLineColor), Arg::bottom(ColorF(UI::JudmentLineBlurColor, 0.5)));
    RectF{ Arg::leftCenter(0, m_judgementYPos), static_cast<double>(FieldSize.x), 3 }.draw(ColorF(UI::JudmentLineColor, 0.6));
    RectF{ Arg::leftCenter(0, m_judgementYPos), static_cast<double>(FieldSize.x), 4 }.stretched(2).drawFrame(0.5).stretched(1).drawFrame(0.5, UI::JudmentLineColor);

}

void Game::drawNote(Note note) const {

    const double cursorY = Cursor::Pos().y * 1000 / Scene::Height();

    RectF{ Arg::leftCenter(EdgeWidth, 1000 - cursorY), m_laneWidth, 15 }.draw(Palette::White);

    RectF{ Arg::leftCenter(EdgeWidth + m_laneWidth, 1100 - cursorY), m_laneWidth, 15 }.draw(Palette::White);
    RectF{ Arg::leftCenter(EdgeWidth + m_laneWidth, 1300 - cursorY), m_laneWidth, 15 }.draw(Palette::White);

    RectF{ Arg::center(EdgeWidth + m_laneWidth + m_laneWidth / 2.0, (1100 + 1300) / 2 - cursorY), m_laneWidth * 0.90, (1300 - 1100) }.draw(ColorF(Palette::White, 0.75));


    if (note.type == 0) {

    }
}
