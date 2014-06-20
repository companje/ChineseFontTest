#include "ofMain.h"
#include "ofxTrueTypeFontUC.h"

class ofApp : public ofBaseApp{
public:

  ofxTrueTypeFontUC font;

  void setup() {
    font.loadFont("arialuni.ttf",40,true);
    //txt.setFont(font);
    ofBackground(0);
  }

  void update() {

  }

  string ofxToString(char ch){
    stringstream sstr;
    sstr << ch;
    return sstr.str();
  }

  void draw() {
    ofTranslate(ofGetWidth()/2,ofGetHeight()/2);

    string txt = "地球公转的中心恒星"; //Hallo ik ben een zin
    ofRectangle bounds = font.getStringBoundingBox(txt,0,0);


    vector<unsigned int> charIDs = font.getCharacterIDs(txt);
    vector<ofMesh> charMeshes = font.getCharacterQuads(txt,0,0);

    if (charIDs.size()!=charMeshes.size()) {
      cout << "charIDs.size()!=charMeshes.size(): " << charIDs.size() << " vs " << charMeshes.size() << endl;
      std::exit(1);
    }

    float radius = 200;
    float antiAliasingScale = 2;
    float fontScale = 1;

    ofScale(fontScale,fontScale);

    ofScale(1/antiAliasingScale,1/antiAliasingScale);
    float scaledRadius = (1/fontScale) * radius * antiAliasingScale;

    enum Alignment { LEFT, CENTER, RIGHT } align = CENTER;
    float angle = 0;
    float px=0;
//    float scaledRadius = 200;

    for (int i=0; i<charIDs.size(); i++) {
      ofMesh &mesh = charMeshes.at(i);

      ofVec3f &a = mesh.getVertices().at(0);
      ofVec3f &b = mesh.getVertices().at(1);
      ofVec3f &c = mesh.getVertices().at(2);
      ofVec3f &d = mesh.getVertices().at(3);

      float x = b.x;
      float w = x-px;
      float z = i;
      angle += atan(w/scaledRadius);
      px = x;

      ofxTranslate(mesh,ofVec3f(-b.x,0,z));
      ofxTranslate(mesh,ofVec3f(0,scaledRadius,0));
      ofxRotate(mesh,-ofRadToDeg(angle),ofVec3f(0,0,1));
    }

    switch (align) {
      case LEFT: break;
      case CENTER: ofRotateZ(ofRadToDeg(angle/2)); break;
      case RIGHT: ofRotateZ(ofRadToDeg(angle)); break;
    }

    int nCopies = 3;

    for (int j=0; j<nCopies; j++) {
      ofPushMatrix();
      ofRotateZ(float(j)/nCopies*360);
      ofTranslate(0,0,j); //float(i)/nCopies*1);

      for (int i=0; i<charIDs.size(); i++) {
        font.bind(charIDs.at(i));
        charMeshes.at(i).draw();
      }

      ofPopMatrix();
    }

  }

  void ofxTranslate(ofMesh &mesh, ofVec3f pos) {
      for (int i=0; i<mesh.getNumVertices(); i++) {
        mesh.getVertices()[i] += pos;
      }
  }

  void ofxTranslate(ofMesh &mesh, float x, float y, float z=0) {
      for (int i=0; i<mesh.getNumVertices(); i++) {
        mesh.getVertices()[i] += ofVec3f(x,y,z);
      }
  }

  void ofxRotate(ofMesh &mesh, float angle, ofVec3f axis) {
      for (int i=0; i<mesh.getNumVertices(); i++) {
        mesh.getVertices()[i].rotate(angle, axis);
      }
  }

  void ofxScale(ofMesh &mesh, float x, float y, float z) {
      for (int i=0; i<mesh.getNumVertices(); i++) {
        mesh.getVertices()[i].x *= x;
        mesh.getVertices()[i].y *= y;
        mesh.getVertices()[i].z *= z;
      }
  }

};


//========================================================================
int main( ){
	ofSetupOpenGL(600,600,OF_WINDOW);			// <-------- setup the GL context
	ofRunApp(new ofApp());
}
