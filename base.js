function changeLanguage(location){
    var onLanguage = document.getElementById("language");
    var language = onLanguage.options[onLanguage.selectedIndex].value;
    localStorage.setItem("language", language);
    localStorage.setItem("changeQuote", false);
    window.location.replace(`${location}index.html`);
}