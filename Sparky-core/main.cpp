#if 0
#include "src/graphics/window.h"
#include "src/graphics/shader.h"
#include "src/maths/maths.h"
#include "src/utils/timer.h"

#include "src/graphics/buffers/buffer.h"
#include "src/graphics/buffers/indexbuffer.h"
#include "src/graphics/buffers/vertexarray.h"

#include "src/graphics/renderer2d.h"
//#include "src/graphics/simple2drenderer.h"
#include "src/graphics/batch2drenderer.h"

//#include "src/graphics/static_sprite.h"
#include "src/graphics/sprite.h"

#include "tilelayer.h"
#include "src/graphics/layers/group.h"
#include "src/graphics/texture.h"
#include "src/graphics/label.h"

#include "src/graphics/font_manager.h"
#include "src/audio/sound_manager.h"

int main() {

	using namespace sparky;
	using namespace graphics;
	using namespace maths;
	using namespace audio;

	Window window("Sparky!", 960, 540);
	//glClearColor(0.5f, 0.05f, 0.0f, 0.5f);

	mat4 ortho = mat4::orthographic(0.0f, 16.0f, 0.0f, 9.0f, -1.0f, 1.0f);
	
	Shader shader ("src/shaders/basic.vert", "src/shaders/basic.frag");
	shader.enable();
	shader.setUniform2f("light_pos", vec2(4.0f, 1.5f));

	TileLayer layer(&shader);

	Texture* textures[] = {
		new Texture("textures/test2.bmp"),
		new Texture("textures/test.png"),
		new Texture("textures/test3.bmp"),
		new Texture("textures/test4.bmp")
	};

	for (float y = -9.0f; y < 9.0f; y ++) {
		for (float x = -16.0f; x < 16.0f; x ++) { 
			//layer.add(new Sprite(x, y, 0.9f, 0.9f, maths::vec4(1, rand() % 1000 / 1000.0f, 0, 1)));
			int r = rand() % 256;

			int col = 0xffff00 << 8 | r;
			if (rand() % 4 == 0)
			{
				layer.add(new Sprite(x, y, 0.9f, 0.9f, col));
			}
			
			layer.add(new Sprite(x, y, 0.9f, 0.9f, textures[rand() % 4]));
		}
	}

	//FontManager::add(new Font("Arial", "Arial.ttf", 32));

	Group* g = new Group(maths::mat4::translation(maths::vec3(-15.8f, 7.2f, 0.0f)));
	Label* fps = new Label("", 0.4f, 0.4f, 0xffffffff);
	g->add(new Sprite(0, 0, 5, 1.5f, 0x505050DD));
	g->add(fps);

	layer.add(g);

	shader.enable();
	shader.setUniformMat4("pr_matrix", maths::mat4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));

	SoundManager::add(new Sound("DogBark", "dogBark.wav"));
	SoundManager::get("DogBark")->play();

	Timer time;                          
	float timer = 0;
	unsigned int frames = 0;
	float t = 0.0f;

	float gain = 0.5f;
	SoundManager::get("DogBark")->setGain(gain);

	while (!window.closed()) {

		t += 0.01f;
		window.clear();
		double x, y;
		window.getMousePosition(x, y);

		shader.enable();
		shader.setUniform2f("light_pos", vec2((float)(x * 32.0f / window.getWidth() - 16.0f), (float)(9.0f - y * 18.0f / window.getHeight())));
		layer.render();

		const std::vector <Renderable2D*>& rs = layer.getRenderables();
		for (int i = 0; i < rs.size(); i++) {
			float c = sin(t) / 2 + 0.5;
			rs[i]->setColor(maths::vec4(c, 0.5, 0.5, 1));
		}

		if (window.isKeyTyped(GLFW_KEY_P))
		{
			SoundManager::get("DogBark")->play();
			std::cout << "Replayed" << std::endl;
		}

		if (window.isKeyTyped(GLFW_KEY_S))
		{
			SoundManager::get("DogBark")->stop();
			std::cout << "Stopped" << std::endl;
		}

		if (window.isKeyTyped(GLFW_KEY_L))
		{
			SoundManager::get("DogBark")->loop();
			std::cout << "Looped" << std::endl;
		}

		if (window.isKeyTyped(GLFW_KEY_1))
		{
			SoundManager::get("DogBark")->pause();
			std::cout << "Paused" << std::endl;
		}

		if (window.isKeyTyped(GLFW_KEY_2))
		{
			SoundManager::get("DogBark")->resume();
			std::cout << "Resume" << std::endl;
		}

		if (window.isKeyTyped(GLFW_KEY_UP))
		{	
			gain += 0.05f;
			SoundManager::get("DogBark")->setGain(gain);
			std::cout << "Gained 1" << std::endl;
		}

		if (window.isKeyTyped(GLFW_KEY_DOWN))
		{
			gain -= 0.05f;
			SoundManager::get("DogBark")->setGain(gain);
			std::cout << "Lost 1" << std::endl;
		}


		SoundManager::update();
		window.update();
		frames++;

		if (time.elapsed() - timer > 1.0f) {
			timer += 1.0f;
			fps->text = std::to_string(frames) + " fps";
			printf("%d fps\n", frames);
			frames = 0;
		}
	}

	delete[] textures;
	//FontManager::clean();
	return 0;
}
#endif
