
#include "bubble.h"
#include "bubble_entry_point.h"

#include "kernel.h"
#include "main_window.h"
#include "image_window.h"


void blur_func(Framebuffer& framebuffer, Framebuffer& buffer, Shader& gauss, const Ref<VertexArray>& FullScreenQuadVAO);

struct MyApplication : Application
{
	MyApplication()
		: Application("Image segmentation")
	{}

	void OnCreate()
	{
		UI::AddModule(CreateScope<TestWindow>());

		//Ref<Shader> shader = ShaderLoader::Load("resources/shaders/convolution.glsl");
		Ref<Shader> shader = ShaderLoader::Load("C:/Users/lol/Desktop/GraduateWork/graduate_work/resources/shaders/convolution.glsl");

		Texture2DSpecification spec;
		spec.WrapS = GL_MIRRORED_REPEAT;
		spec.WrapT = GL_MIRRORED_REPEAT;

		Texture2D image("resources/images/nature.jpg", spec);
		Framebuffer fb(image.GetWidth(), image.GetHeight());

		Kernel kernel = { {0.1f, 0.1f, 0.1f }, { 0.1f, 0.1f, 0.1f }, { 0.1f, 0.1f, 0.1f } };
		kernel.Bind();

		fb.Bind();
		shader->SetTexture2D("uImage", image);
		Renderer::DrawIndices(Renderer::sFullScreenVAO);

		UI::AddModule<ImageWindow>(std::move(fb.GetColorAttachment()));

		// OpenCL temp
		//std::vector<cl::Platform> platforms;
		//cl::Platform::get(&platforms);
		//
		//std::vector<cl::Device> devices;
		//platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);
		//
		//cl::string name = devices[0].getInfo<CL_DEVICE_VENDOR>();
	}

	void OnUpdate(Bubble::DeltaTime dt)
	{

	}

	void OnEvent(const SDL_Event& event)
	{

	}

};

Application* CreateApplication()
{
	return new MyApplication;
}


void blur_func(Framebuffer& framebuffer, Framebuffer& buffer, Shader& gauss, const Ref<VertexArray>& FullScreenQuadVAO)
{
	
}