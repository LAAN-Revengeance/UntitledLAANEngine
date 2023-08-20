
--Loads all resources used in game
function load_resources()

	--load textures
	resources:LoadTexture("grass","resources/textures/terrain/grass.png");
	resources:LoadTexture("rock","resources/textures/terrain/rock.png");
	resources:LoadTexture("dirt","resources/textures/terrain/dirt.png");
	resources:LoadTexture("heightMap","resources/textures/heightmap/heightmap512_rivers.png");
	resources:LoadTexture("detailMap","resources/textures/terrain/detail.png");
	resources:LoadTexture("black","resources/textures/terrain/black.png");
	resources:LoadTexture("grid","resources/textures/terrain/tile_Specular2.png");

	
	resources:LoadCubemap("skybox",
		"resources/textures/skybox/Synthwave3/Right.png",
		"resources/textures/skybox/Synthwave3/Left.png",
		"resources/textures/skybox/Synthwave3/Top.png",
		"resources/textures/skybox/Synthwave3/Bottom.png",
		"resources/textures/skybox/Synthwave3/Front.png",
		"resources/textures/skybox/Synthwave3/Back.png"
	);
	
end
