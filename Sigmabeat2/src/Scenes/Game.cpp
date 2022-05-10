#include "Game.hpp"

Game::Game(const InitData& init)
    : IScene(init)
    , m_fieldRT(FieldSize)
    , VS(HLSL{ U"example/shader/hlsl/homography.hlsl", U"VS" } | GLSL{ U"example/shader/glsl/homography.vert", {{ U"VSConstants2D", 0 }, { U"VSHomography", 1} } })
    , PS(HLSL{ U"example/shader/hlsl/homography.hlsl", U"PS" } | GLSL{ U"example/shader/glsl/homography.frag", {{ U"PSConstants2D", 0 }, { U"PSHomography", 1} } }) {

    Scene::SetBackground(Palette::Whitesmoke);
    Scene::SetLetterbox(Palette::Whitesmoke);

    if ((not VS) || (not PS)) {
        throw Error{ U"Failed to load shader files" };
    }

}

void Game::update() {

}

void Game::draw() const {
    drawLane();
}

void Game::drawLane() const {

    auto [SceneW, SceneH] = Scene::Size();

    auto TopW = SceneW * 0.20;
    auto BottomW = SceneW * 0.70;

    Quad quad{ Vec2{(SceneW - TopW) / 2, 0}, Vec2{(SceneW + TopW) / 2, 0}, Vec2{(SceneW + BottomW) / 2, SceneH}, Vec2{(SceneW - BottomW) / 2, SceneH} };

    {
        const ScopedRenderTarget2D target(m_fieldRT.clear(Palette::Black));

        const uint8 N = 6;
        const int32 EdgeWidth = 30;
        double LaneWidth = (FieldSize.x - 2 * EdgeWidth) / N;

        for (uint8 i = 1; i < N; i++) {
            RectF{ Arg::topCenter(EdgeWidth + LaneWidth * i, 0.0), 2.0, (double)FieldSize.y }.draw(Palette::White);
        }

        // judge Line
        Rect{ 500, 8 }.movedBy(0, Cursor::Pos().y * 1000 / SceneH).draw(Palette::Red);

        Rect{ Arg::topRight(FieldSize.x, 0), EdgeWidth, FieldSize.y }.draw(Palette::White);
        Rect{ Arg::topLeft(0, 0), EdgeWidth, FieldSize.y }.draw(Palette::White);

        Rect{ FieldSize }.draw(Arg::top = ColorF(0, 0, 0, 0.8), Arg::bottom = ColorF(0, 0, 0, 0));

    }


    /*--------------------------*/

    Graphics2D::Flush();
    m_fieldRT.resolve();

    // debug
    RectF{ FieldSize * 0.65 }(m_fieldRT).draw();

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
}
