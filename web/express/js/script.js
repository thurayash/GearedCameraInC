

let mainVideo = document.getElementById('myVideo');


document.addEventListener('DOMContentLoaded', () => {

    // Get all "navbar-burger" elements
    const $navbarBurgers = Array.prototype.slice.call(document.querySelectorAll('.navbar-burger'), 0);

    // Check if there are any navbar burgers
    if ($navbarBurgers.length > 0) {

        // Add a click event on each of them
        $navbarBurgers.forEach( el => {
            el.addEventListener('click', () => {

                // Get the target from the "data-target" attribute
                const target = el.dataset.target;
                const $target = document.getElementById(target);

                // Toggle the "is-active" class on both the "navbar-burger" and the "navbar-menu"
                el.classList.toggle('is-active');
                $target.classList.toggle('is-active');

            });
        });
    }

});

//tabs
const tabs = document.querySelectorAll('.tabs li');
const tabContentBoxes = document.querySelectorAll('#tab-content > div');
const ex = document.getElementById("tab-container");
//const divs = ex.getElementsByTagName('div');

tabs.forEach(tab => {
    tab.addEventListener('click', () => {
        tabs.forEach(item => item.classList.remove('is-active'));
        tabs.forEach(item => item.style.color = "white");
        tab.classList.add('is-active');
        tab.style.color = "#3298dc";
        const target = tab.dataset.target;
        // console.log(target);
        for(i = 0; i < divs.length; i++)
        {
            if(divs[i].id == target)
            {
                divs[i].style.display = 'block';
            }
            else{
                divs[i].style.display = 'none' ;

            }
        }
    })
});

var isMobile = {
    Android: function() {
        return navigator.userAgent.match(/Android/i);
    },
    BlackBerry: function() {
        return navigator.userAgent.match(/BlackBerry/i);
    },
    iOS: function() {
        return navigator.userAgent.match(/iPhone|iPad|iPod/i);
    },
    Opera: function() {
        return navigator.userAgent.match(/Opera Mini/i);
    },
    Windows: function() {
        return navigator.userAgent.match(/IEMobile/i) || navigator.userAgent.match(/WPDesktop/i);
    },
    any: function() {
        return (isMobile.Android() || isMobile.BlackBerry() || isMobile.iOS() || isMobile.Opera() || isMobile.Windows());
    }
};


function sleep(milliseconds) {
  const date = Date.now();
  let currentDate = null;
  do {
    currentDate = Date.now();
  } while (currentDate - date < milliseconds);
}


function MobileTabs(id){
  if (isMobile.any())
  {
    var ul = document.getElementById("tabs");
    var element = document.getElementById(id);
    ul.appendChild(element);
  }
}



//disableScroll();

mainVideo.addEventListener('ended',myHandler,false);
    function myHandler(e) {
        let element = document.getElementById("heroCont");
        element.setAttribute("style", "animation-name: image_blur;animation-duration : 5s;animation-fill-mode:forwards;");
        sleep(500);        
        document.getElementById("title").setAttribute("style", "animation-name: text_appear;animation-duration : 3s;animation-fill-mode:forwards;");
        activate();
        //enableScroll();

}


function activate()
{
    new TypeIt("#description", {
      speed: 50,
      waitUntilVisible: true,
      loop: true
    })
      .type("GCC The most ", {delay: 300})      
      .type('<b>efficient</b> camera ever')
      .pause(600)
      .move(-12)
      .delete(9)
      .type('<b>funny</b>', {delay: 300})
      .pause(600)
      .delete(5)
      .type('<b>friendly</b>',{delay:300})
      .move('END')
      .type(' &#128515;',{delay :300})
      .pause(50000)
      .go();
}

window.addEventListener('scroll', function()
{
    let value = 1 + window.scrollY/-600;
    mainVideo.style.opacity = value;
});


function disableScroll() {
document.body.style.overflow = 'hidden';
document.querySelector('html').scrollTop = window.scrollY;
}

function enableScroll() {
document.body.style.overflow = null;
}