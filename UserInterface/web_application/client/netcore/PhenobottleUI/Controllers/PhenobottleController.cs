using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using PhenobottleUI.Models;

namespace PhenobottleUI.Controllers
{
    public class PhenobottleController : Controller
    {

        private readonly APIContext _db;

        public PhenobottleController(APIContext db) {
            _db = db;
		}

        public IActionResult Index()
        {
            IEnumerable<Phenobottle> objList = _db.Phenobottles;
            return View(objList);
        }
    }
}