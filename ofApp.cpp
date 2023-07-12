#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(3);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	auto noise_seed = glm::vec2(ofRandom(1000), ofRandom(1000));
	for (int k = 0; k < 1; k++) {

		ofMesh mesh;
		vector<glm::vec3> right, left, frame;

		glm::vec3 last_location;
		float last_theta;

		for (int i = 0; i < 24; i++) {

			auto radius = ofMap(ofNoise((ofGetFrameNum() + i + k * 10) * 0.04), 0, 1, 0, 380);
			auto next_radius = ofMap(ofNoise((ofGetFrameNum() + i + 1 + k * 10) * 0.04), 0, 1, 0, 380);

			auto deg = ofMap(ofNoise(k, (ofGetFrameNum() + i) * 0.0085), 0, 1, -180, 180);
			auto next_deg = ofMap(ofNoise(k, (ofGetFrameNum() + i + 1) * 0.0085), 0, 1, -180, 180);

			auto location = glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), 0);
			auto next = glm::vec3(next_radius * cos(next_deg * DEG_TO_RAD), next_radius * sin(next_deg * DEG_TO_RAD), 0);

			auto direction = next - location;
			auto theta = atan2(direction.y, direction.x);

			right.push_back(location + glm::vec3(ofMap(i, 0, 25, 0, 8) * cos(theta + PI * 0.5), ofMap(i, 0, 25, 0, 8) * sin(theta + PI * 0.5), 0));
			left.push_back(location + glm::vec3(ofMap(i, 0, 25, 0, 8) * cos(theta - PI * 0.5), ofMap(i, 0, 25, 0, 8) * sin(theta - PI * 0.5), 0));

			last_location = location;
			last_theta = theta;
		}

		for (int i = 0; i < right.size(); i++) {

			mesh.addVertex(left[i]);
			mesh.addVertex(right[i]);
		}


		for (int i = 0; i < mesh.getNumVertices() - 2; i += 2) {

			mesh.addIndex(i + 0); mesh.addIndex(i + 1); mesh.addIndex(i + 3);
			mesh.addIndex(i + 0); mesh.addIndex(i + 2); mesh.addIndex(i + 3);
		}

		mesh.addVertex(last_location);
		int index = mesh.getNumVertices();
		for (auto theta = last_theta - PI * 0.5; theta <= last_theta + PI * 0.5; theta += PI / 20) {

			mesh.addVertex(last_location + glm::vec3(8 * cos(theta), 8 * sin(theta), 0));
			frame.push_back(last_location + glm::vec3(8 * cos(theta), 8 * sin(theta), 0));
		}

		for (int i = index; i < mesh.getNumVertices() - 1; i++) {

			mesh.addIndex(index); mesh.addIndex(i + 0); mesh.addIndex(i + 1);
		}

		reverse(right.begin(), right.end());
		ofColor color;
		for (int i = 0; i < 20; i++) {

			ofRotate(18);

			ofSetColor(255);
			mesh.draw();

			color.setHsb(ofMap(i, 0, 20, 0, 255), 150, 255);
			ofSetColor(color);
			ofNoFill();
			ofBeginShape();
			ofVertices(frame);
			
			ofVertices(right);
			ofVertices(left);
			ofEndShape(false);
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}