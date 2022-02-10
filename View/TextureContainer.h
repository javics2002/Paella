//
// Created by eys on 24/8/21.
//

#ifndef CARGAME_TEXTURECONTAINER_H
#define CARGAME_TEXTURECONTAINER_H

#include <array>
#include <string>

#include "Texture.h"

using namespace std;

const string IMAGES_PATH = "../../../Images/";

enum TextureName {
    berenjenaTexture,
    inicioCintaTexture,
    finalCintaTexture,
    ingrAlcachofa,
    ingrCalamar,
    ingrCangrejo,
    ingrGamba,
    ingrLangosta,
    ingrMejillon,
    ingrPimientoRojo,
    ingrPimientoVerde,
    ingrPollo
};

const int NUM_TEXTURES = 12;

typedef struct {
    string filename;
    int numRows; int numCols;
} TextureAttributes;


const TextureAttributes TEXTURE_ATTRIBUTES[NUM_TEXTURES] =
          { {"berenjena.jpg",1,1},
            {"inicioCinta.png",1,1},
            {"finalCinta.png",1,1},

            {"/Ingredientes/alcachofa.png",1,1},
            {"/Ingredientes/calamar.png",1,1},
            {"/Ingredientes/cangrejo.png",1,1},
            {"/Ingredientes/gamba.png",1,1},
            {"/Ingredientes/langosta.png",1,1},
            {"/Ingredientes/mejillon.png",1,1},
            {"/Ingredientes/pimiento_rojo.png",1,1},
            {"/Ingredientes/pimiento_verde.png",1,1},
            {"/Ingredientes/pollo.png",1,1} };

class TextureContainer {

    array<Texture*, NUM_TEXTURES> textures;

public:
    TextureContainer(SDL_Renderer* renderer) {
        for (int i = 0; i < NUM_TEXTURES; i++) {
            const TextureAttributes& attributes = TEXTURE_ATTRIBUTES[i];
            textures[i] = new Texture(renderer, IMAGES_PATH + attributes.filename, attributes.numRows, attributes.numCols);
        }
    }

    ~TextureContainer() {
        for (int i = 0; i < NUM_TEXTURES; i++) {

            if (textures[i] != nullptr) {
                textures[i]->free();
            }
            delete textures[i];
        }
    }

    Texture* getTexture(TextureName name) const { return textures[name]; };

};

#endif //CARGAME_TEXTURECONTAINER_H

