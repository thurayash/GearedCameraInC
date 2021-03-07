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
const divs = ex.getElementsByTagName('div');

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

