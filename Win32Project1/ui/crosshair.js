var game=new Phaser.Game(96, 96, Phaser.WEBGL, 'gameDiv', { preload: preload, create: create, update: update },true);

function preload() {

    game.load.spritesheet('crosshair', 'myUI/crosshair.png', 96, 96);

}

var varCrosshair;
function create() {
    varCrosshair=game.add.sprite(0,0,'crosshair');
}
function update() 
{	

}