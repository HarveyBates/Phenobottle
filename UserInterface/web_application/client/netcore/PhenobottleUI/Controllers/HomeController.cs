using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using PhenobottleUI.Models;

namespace PhenobottleUI.Controllers
{
    public class HomeController : Controller
    {
        private readonly ILogger<HomeController> _logger;

        public HomeController(ILogger<HomeController> logger)
        {
            _logger = logger;
        }

        [HttpPost]
        [ValidateAntiForgeryToken]
        public IActionResult ProcessButton(Phenobottle obj, EventArgs e) {
            _logger.LogInformation(obj.ID);
            return View("Home", obj);
        }

        public IActionResult Home() {
            
            return View();
		}

        public IActionResult Dashboard() {
            return View();
		}

        public IActionResult Phenobottles() {
            return View();
		}

        public IActionResult Setup() {
            return View();
		}

        public IActionResult Environment() {
            return View();
		}

        public IActionResult Settings() {
            return View();
		}

        [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
        public IActionResult Error()
        {
            return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
        }

    }
}
