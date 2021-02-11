using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;

namespace PhenobottleUI.Controllers
{
    public class ExperimentController : Controller
    {
        public IActionResult Index()
        {
            return View();
        }
    }
}