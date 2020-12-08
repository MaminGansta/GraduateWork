
#include "bubble.h"

#include "ui/main_window.h"
#include "UI/image_window.h"

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
		Texture2D image2(image.GetWidth(), image.GetHeight());

		Framebuffer res(std::move(image));
		Framebuffer fb(std::move(image2));
		
		blur_func(res, fb, *shader, Renderer::sFullScreenVAO);

		auto image3 = res.GetColorAttachment();
		auto image_copy = Renderer::CopyTexture2D(image3);

		UI::AddModule<ImageWindow>(std::move(image3));
		UI::AddModule<ImageWindow>(std::move(image_copy));
		UI::AddModule<ImageWindow>(fb.GetColorAttachment());

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
	buffer.Bind();
	gauss.SetTexture2D("uImage", framebuffer.GetColorAttachmentRendererID());
	Renderer::DrawIndices(FullScreenQuadVAO);
}