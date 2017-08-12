var game=new Phaser.Game(500, 500, Phaser.AUTO, 'gameDiv', { preload: preload, create: create, update: update },true);

function preload() {

    game.load.spritesheet('btnPlay', 'myUI/btnPlay.png', 310, 90);
    game.load.spritesheet('btnQuit', 'myUI/btnQuit.png', 310, 90);

}

var buttonPlay;
var buttonQuit;
var background;

function create() {


    buttonPlay = game.add.button(0, 0, 'btnPlay',null,this,0,0,1);


    buttonQuit = game.add.button(0,90, 'btnQuit',null,this,0,0,1);

}
function update() 
{	

}