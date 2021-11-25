import * as THREE from "three"

class ColorGUIController {

  constructor(object, prop) {
    this.object = object
    this.prop = prop
  }

  get value() {
    // i don't no why, but without copy of array it isn't working :)
    return [...this.object[this.prop].value.map( it => it * 255 )]
  }

  set value(newbie) {
    this.object[this.prop] = new THREE.Uniform(newbie.map( it => it / 255 ))
  }
}

export { ColorGUIController }