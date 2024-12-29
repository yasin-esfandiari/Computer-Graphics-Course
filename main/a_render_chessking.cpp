#include <core/assert.h>
#include <core/scalar.h>
#include <core/image.h>
#include <rt/world.h>
#include <rt/renderer.h>
#include <rt/loaders/obj.h>
#include <rt/groups/bvh.h>
#include <rt/groups/simplegroup.h>
#include <rt/solids/sphere.h>
#include <rt/cameras/perspective.h>
#include <rt/cameras/dofperspective.h>
#include <rt/integrators/casting.h>
#include <rt/lights/directional.h>
#include <rt/lights/spotlight.h>
#include <rt/primmod/instance.h>
#include <core/matrix.h>

#include <rt/textures/constant.h>
#include <rt/textures/imagetex.h>

#include <rt/materials/lambertian.h>
#include <rt/materials/flatmaterial.h>
#include <rt/materials/fuzzyconductor.h>
#include <rt/materials/dielectric.h>
#include <rt/materials/phong.h>
#include <rt/materials/conductor.h>
#include <rt/materials/combine.h>

#include <rt/solids/quad.h>
#include <rt/solids/triangle.h>
#include <rt/solids/sphere.h>

#include <rt/coordmappers/plane.h>
#include <rt/coordmappers/cylindrical.h>
#include <rt/coordmappers/spherical.h>
#include <rt/coordmappers/tmapper.h>

#include <rt/integrators/recraytrace.h>
#include <rt/lights/arealight.h>
#include <rt/lights/spotlight.h>
#include <rt/lights/pointlight.h>
#include <core/random.h>
#include <rt/solids/disc.h>

using namespace rt;

void a_render_chessking() {

    Image img(400, 400);
    float aspect = img.width() / (float)img.height();

    // Texture *greentex = new ConstantTexture(RGBColor(0.0f, 0.7f, 0.0f));
    // Texture *bluetex = new ConstantTexture(RGBColor(0.0f, 0.0f, 0.7f));
    // Texture *pinktex = new ConstantTexture(RGBColor(1.0f, 0.71f, 0.75f));
    // Texture *whitetex = new ConstantTexture(RGBColor(1.0f, 1.0f, 1.0f));
    // Texture *blacktex = new ConstantTexture(RGBColor(0.0f, 0.0f, 0.0f));
    // Texture *graytex = new ConstantTexture(RGBColor(0.1f, 0.1f, 0.1f));
    // Texture *orangetex = new ConstantTexture(RGBColor(1.0f, 0.84f, 0.0f));
    // Texture *redtex = new ConstantTexture(RGBColor(1.0f, 0, 0.0f));
    // Texture *goldtex = new ConstantTexture(RGBColor(0.9f, 0.9f, 0.0f));
    // Texture *light_bluetex = new ConstantTexture(RGBColor(0.01f, 0.01f, 0.03f));
    Texture *blacktex = new ConstantTexture(RGBColor::rep(0.0f));
    Texture *plane_emission = NULL; // new ConstantTexture(RGBColor(0.5f, 0.5f, 0.5f));
    Texture *plane_texture = new ConstantTexture(RGBColor::rep(0.075f));
    Texture *pawn_emission = NULL; // new ConstantTexture(RGBColor(0.18f, .1f, 0.02f));
    Texture *pawn_texture = new ConstantTexture(RGBColor(0.04f, 0.04f, 0.04f));

    // Material *white_mat = new LambertianMaterial(NULL, whitetex);
    // Material *black_mat = new PhongMaterial(blacktex, 1);
    // Material *orange_mat = new LambertianMaterial(NULL, orangetex);
    // Material *blue_mat = new LambertianMaterial(NULL, bluetex);
    // Material *gold_mat = new LambertianMaterial(NULL, goldtex);
    // Material *green_mat = new LambertianMaterial(NULL, greentex);
    // Material *red_mat = new LambertianMaterial(NULL, redtex);
    // Material *pink_mat = new LambertianMaterial(NULL, pinktex);
    // Material *floor_mat = new FuzzyMirrorMaterial(2.485f, 3.433f, 0.05f);
    // Material *mirror_glass_mat = new LambertianMaterial(NULL, yellowtex);
    // Material *floorMaterial = new LambertianMaterial(blacktex, whitetex);
    // Material *brown_mat = new LambertianMaterial(NULL, browntex);
    // Material *glass_mat = new GlassMaterial(1.0f);
    // Material *yellow_mat = new LambertianMaterial(NULL, yellowtex);
    // Material *light_blue_mat = new LambertianMaterial(light_bluetex, light_bluetex);
    Material *plane_material = new LambertianMaterial(plane_emission, plane_texture);
    Material *pawn_material = new LambertianMaterial(pawn_emission, pawn_texture);
    Material *king_material = new FuzzyConductorMaterial(2.485f, 2.433f, 0.05f);
    Material *queen_material = new FuzzyConductorMaterial(2.485f, 2.433f, 0.05f);
    Material *rook_material = new DielectricMaterial(1.5f);

    // matlib->insert(std::pair<std::string, Material *>("orange-nose", orange_mat));
    // matlib->insert(std::pair<std::string, Material *>("blue-background", light_blue_mat));
    // matlib->insert(std::pair<std::string, Material *>("white-snow", white_mat));
    // matlib->insert(std::pair<std::string, Material *>("orange-babysnowman-hat", gold_mat));
    // matlib->insert(std::pair<std::string, Material *>("green-tree", green_mat));
    // matlib->insert(std::pair<std::string, Material *>("black-hat", pink_mat));
    // matlib->insert(std::pair<std::string, Material *>("red-hat-stripe", red_mat));
    // matlib->insert(std::pair<std::string, Material *>("yellow-glasses", mirror_glass_mat));
    // matlib->insert(std::pair<std::string, Material *>("blue-mask", blue_mat));
    // matlib->insert(std::pair<std::string, Material *>("rc_pawn_material", brown_mat));
    MatLib *matlib = new MatLib;
    matlib->insert(std::pair<std::string, Material *>("rc_plane_material", plane_material));
    matlib->insert(std::pair<std::string, Material *>("rc_pawn_material", pawn_material));
    matlib->insert(std::pair<std::string, Material *>("rc_king_material", king_material));
    matlib->insert(std::pair<std::string, Material *>("rc_queen_material", queen_material));
    matlib->insert(std::pair<std::string, Material *>("rc_rook_material", rook_material));


    BVH *scene = new BVH();
    // loadOBJ(scene, "../models/", "rc_scene.obj", matlib);
    loadOBJ(scene, "../models/", "rc_plane.obj", matlib);
    loadOBJ(scene, "../models/", "rc_pawn.obj", matlib);
    loadOBJ(scene, "../models/", "rc_king.obj", matlib);
    loadOBJ(scene, "../models/", "rc_queen.obj", matlib);
    loadOBJ(scene, "../models/", "rc_rook.obj", matlib);
    scene->rebuildIndex();

    //light
    World world;
    world.light.push_back(new PointLight(Point(2.5f, 1.5, 1.5), RGBColor::rep(600.f))); // point light
    // world.light.push_back(new PointLight(Point(-2.5f, -1.f, 1.5), RGBColor::rep(50.f))); // point light
    // world.light.push_back(new PointLight(Point(-1.f, -2.5f, 1.5), RGBColor::rep(50.f))); // point light
    
    // Area light
    ConstantTexture *lightsrctex = new ConstantTexture(RGBColor::rep(95.0f));
    Material *lightsource = new LambertianMaterial(lightsrctex, blacktex);
    Quad *light_up = new Quad(Point(-2.75f, -2.75f, 2.5f), Vector(0.f, 1.2f, 0.f), Vector(1.2f, 0.f, 0.f), nullptr, lightsource);
    AreaLight als(light_up);
    world.light.push_back(&als);
    scene->add(light_up);

    // Point cam_o = Point(10.8f, -1.4f, 10.5f);
    Point cam_o = Point(0, 10, 10);
    Vector cam_d = Point(-1.5f, -1, 0) - cam_o;
    float angle = 0.3;
    PerspectiveCamera cam(cam_o, cam_d, Vector(0, 0, 1), angle, angle * aspect);
    // DOFPerspectiveCamera cam(cam_o, cam_d, Vector(0, 0, 1), angle, angle * aspect, 12.f, 0.12f);

    // scene->rebuildIndex();
    world.scene = scene;
    RecursiveRayTracingIntegrator integrator(&world);
    // RayCastingIntegrator integrator(&world);
    int num_samples = 8;
    Renderer engine1(&cam, &integrator);
    engine1.setSamples(num_samples);
    engine1.render(img);
    img.writeEXR("rc.exr");

}