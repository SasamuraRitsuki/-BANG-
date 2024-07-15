#include "../../../../dxlib_ext/dxlib_ext.h"
#include "wall.h"

Wall::Wall() {
	//--------------------------------•Ç1-----------------------------------------
	wall1_mesh_ = dxe::Mesh::CreateCubeMV(WALL_SIZE);
	wall1_mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/green2.jpg"));
	wall1_mesh_->scl_ = WALL1_FORM;
	wall1_mesh_->pos_ = wall1_pos_;
	wall1_mesh_->setCullingMode(DX_CULLING_RIGHT) ;

	wall1_size_v = WALL1_FORM * WALL_SIZE;
	//--------------------------------•Ç2-----------------------------------------
	wall2_mesh_ = wall1_mesh_->createClone();
	wall2_mesh_->scl_ = WALL2_FORM;
	wall2_mesh_->pos_ = wall2_pos_;
	wall2_mesh_->setCullingMode(DX_CULLING_RIGHT);

	wall2_size_v = WALL2_FORM * WALL_SIZE;
	//--------------------------------•Ç3-----------------------------------------
	wall3_mesh_ = wall1_mesh_->createClone();
	wall3_mesh_->scl_ = WALL3_FORM;
	wall3_mesh_->pos_ = wall3_pos_;
	wall3_mesh_->setCullingMode(DX_CULLING_RIGHT);

	wall3_size_v = WALL3_FORM * WALL_SIZE;
	//--------------------------------•Ç4-----------------------------------------
	wall4_mesh_ = wall1_mesh_->createClone();
	wall4_mesh_->scl_ = WALL4_FORM;
	wall4_mesh_->pos_ = wall4_pos_;
	wall4_mesh_->setCullingMode(DX_CULLING_RIGHT);

	wall4_size_v = WALL4_FORM * WALL_SIZE;
	//--------------------------------•Ç5-----------------------------------------
	wall5_mesh_ = wall1_mesh_->createClone();
	wall5_mesh_->scl_ = WALL5_FORM;
	wall5_mesh_->pos_ = wall5_pos_;
	wall5_mesh_->setCullingMode(DX_CULLING_RIGHT);

	wall5_size_v = WALL5_FORM * WALL_SIZE;
	//--------------------------------‰¼‘z•Ç-----------------------------------------
	/*wall_virtual_mesh_ = wall1_mesh_->createClone();
	wall_virtual_mesh_->scl_ = WALL_VIRTUAL_FORM;
	wall_virtual_mesh_->pos_ = wall_virtual_pos_;
	wall_virtual_mesh_->setCullingMode(DX_CULLING_RIGHT);*/

	wall_virtual_size_v = WALL_VIRTUAL_FORM * WALL_SIZE;
}

Wall::~Wall() {

}

void Wall::update(float delta_time) {

}

void Wall::draw(Shared<dxe::Camera> camera) {

	wall1_mesh_->render(camera);
	wall2_mesh_->render(camera);
	
	//‰œ
	wall4_mesh_->render(camera);
	
	wall5_mesh_->render(camera);
	//Žè‘O
	wall3_mesh_->render(camera);

	//wall_virtual_mesh_->render(camera);
}
