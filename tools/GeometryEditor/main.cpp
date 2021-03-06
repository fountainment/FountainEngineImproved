#include "fountain.h"
#include "EditorScene.h"

using namespace fei;

class GeometryEditor : public Application
{
public:
	void engineSetting(Engine* engine)
	{
		engine->getWindow()->setResizable(true);
		engine->getWindow()->setSize(800, 600);
		engine->getWindow()->setTitle("GeometryEditor v0.07");
		engine->getWindow()->setSamples(8);
		engine->getWindow()->getSceneManager()->gotoScene(new EditorScene);
	}
};

int main()
{
	GeometryEditor app;
	app.run();
	return 0;
}
