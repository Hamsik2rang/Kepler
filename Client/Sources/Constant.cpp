#include "Constant.h"


namespace constant {
	const kepler::Vec2f SQUIRTLE_IDLE_SIZE{ 137.25f, 180.0f };
	const kepler::Vec2f SQUIRTLE_WALK_SIZE{ 127.25f, 180.0f };
	const kepler::Vec2f SQUIRTLE_JUMP_SIZE{ 120.3f, 180.0f };
	const kepler::Vec2f SQUIRTLE_SLIDE_SIZE{ 222.0f, 120.0f };
	const kepler::Vec2f SQUIRTLE_WIN_SIZE{ 205.92f, 180.0f };
	const kepler::Vec2f SQUIRTLE_LOSE_SIZE{ 257.22f, 120.0f };

	const kepler::Vec2f PLAYER_SPAWN_POSITION{ 360.0f, -230.0f };

	const kepler::Vec2f NET_POSITION{ 0.0f, -235.0f };
	const kepler::Vec2f NET_SIZE{ 10.0f, 360.0f };

	const kepler::Vec2f BALL_SIZE{ 150.0f, 150.0f };
	const kepler::Vec2f BALL_PLAYER_SPAWN_POSITION{ 360.0f, 220.0f };
	const kepler::Vec2f BALL_ENEMY_SPAWN_POSITION{ -362.0f, 220.0f };
	const kepler::Vec4f BALL_SHADOW{ 13.0f / 255.0f, 9.0f / 255.0f, 240.0f / 255.0f, 1.0f };

	const kepler::Vec2f GROUND_POSITION{ 0.0f, -320.0f };
	const kepler::Vec2f GROUND_SIZE{ 50000.0f, 5.0f };

	const kepler::Vec2f SKY_POSITION{ 0.0f, 400.0f };
	const kepler::Vec2f SKY_SIZE{ 50000.0f, 5.0f };
}
