var game=new Phaser.Game(310, 180, Phaser.WEBGL, 'gameDiv', { preload: preload, create: create, update: update },true);

function preload() {

    game.load.spritesheet('btnPlay', 'myUI/btnPlay.png', 310, 90);
    game.load.spritesheet('btnQuit', 'myUI/btnQuit.png', 310, 90);

}

var buttonPlay;
var buttonQuit;

function create() {

   buttonPlay = game.add.button(0, 0, 'btnPlay',play,this,0,0,1);
    buttonQuit = game.add.button(0,90, 'btnQuit',quit,this,0,0,1);

}

function play()
{
    myObject.play();
}

function quit()
{
    myObject.quit();
}

function update() 
{	

}