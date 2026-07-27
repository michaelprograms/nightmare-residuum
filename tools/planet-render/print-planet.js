import * as fs from 'fs';

import * as pureimage from 'pureimage';
import shadeblendcolor from 'shade-blend-color';
let pSBC = shadeblendcolor.default;

// Usage:
// node print-planet.js [path to planet json]
// ex:
// node print-planet.js ../../lib/tmp/[Planet].json

console.time(`Planet generate png`);

let file, output;
if (process.argv.length > 2) {
  file = process.argv[2];
  output = file.replace('.json', '');
} else {
  console.error('No filename provided');
  process.exit(-1);
}

let d;
try {
  const data = fs.readFileSync(file, 'utf8');
  d = JSON.parse(data);
} catch (err) {
  console.log(`Error reading file from disk: ${err}`);
  process.exit(0);
}

let size = +d.size;
let image = pureimage.make(size * 2, size * 4);
let ctx = image.getContext('2d');
for (let y = 0; y < size; y ++) {
    for (let x = 0; x < size; x ++) {
      const thisPixel = d.data[y][x];

      // Biome
      let thisColor = thisPixel[0];
      if (typeof thisColor === 'number') {
        ctx.fillStyle = pSBC(thisColor, '#000000', '#FFFFFF', true);
      } else {
        ctx.fillStyle = thisColor;
      }
      ctx.fillRect(x, y, 1, 1);                    // top-left: plain biome
      ctx.fillRect(x + size, y + size * 3, 1, 1);  // row 4 right: biome (markers added below)

      // Height
      thisColor = thisPixel[1];
      if (typeof thisColor === 'number') {
        ctx.fillStyle = pSBC(thisColor, '#000000', '#FFFFFF', true);
      } else {
        ctx.fillStyle = thisColor;
      }
      ctx.fillRect(x + size, y, 1, 1);

      // Humidity
      thisColor = thisPixel[2];
      if (typeof thisColor === 'number') {
        ctx.fillStyle = pSBC(thisColor, '#000000', '#FFFFFF', true);
      } else {
        ctx.fillStyle = thisColor;
      }
      ctx.fillRect(x, y + size, 1, 1);

      // Heat
      thisColor = thisPixel[3];
      if (typeof thisColor === 'number') {
        ctx.fillStyle = pSBC(thisColor, '#000000', '#FFFFFF', true);
      } else {
        ctx.fillStyle = thisColor;
      }
      ctx.fillRect(x + size, y + size, 1, 1);

      // Level
      thisColor = thisPixel[4];
      if (typeof thisColor === 'number') {
        ctx.fillStyle = pSBC(thisColor, '#FFFFFF', '#7F0000', true);
      } else {
        ctx.fillStyle = thisColor;
      }
      ctx.fillRect(x, y + size * 2, 1, 1);

      // Resource
      thisColor = thisPixel[5];
      if (typeof thisColor === 'number') {
        ctx.fillStyle = pSBC(thisColor, '#000000', '#FFFFFF', true);
      } else {
        ctx.fillStyle = thisColor;
      }
      ctx.fillRect(x + size, y + size * 2, 1, 1);
    }
}

// Row 4: structure views. Left is a black field with just the markers
// (like the resource panel); right already holds the biome, so markers
// go on top of it. Each entry is [x, y, type, level]; d.structures is
// absent until the daemon emits it, so treat missing as "no markers".
ctx.fillStyle = '#000000';
ctx.fillRect(0, size * 3, size, size);

const structureColor = { crashed_ship: '#FF00FF', ruin: '#FF0000' };
for (const s of d.structures ?? []) {
  const [sx, sy, type] = s;
  ctx.fillStyle = structureColor[type] ?? '#FFFFFF';
  ctx.fillRect(sx, sy + size * 3, 1, 1);         // structures on black
  ctx.fillRect(sx + size, sy + size * 3, 1, 1);  // structures on biome
}

await pureimage.encodePNGToStream(image, fs.createWriteStream(output + '-' + size + '.png'));
console.info('File: ' + output + '-' + size + '.png');
console.timeEnd(`Planet generate png`);
