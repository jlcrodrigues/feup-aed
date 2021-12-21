#include "App.h"

App::App(): ITEMS_PER_PAGE(5)
{
   app_run = true;
}

App::App(const string &airports, const string &flights, const string &passengers, const string &planes, const string& carts): ITEMS_PER_PAGE(5)
{
   airline = Airline(airports, flights, passengers, planes, carts);
   app_run = true;
}

void App::run()
{
   cout << "Welcome to Airline Manager. Use help to get started.\n";
   while (app_run)
   {
      cout << ">";
      readCommand();
      processCommand();
   }
}

void App::clearStream() const
{
   cin.clear();
   cin.ignore(INT_MAX, '\n');
}

bool App::readNumber(int& n, const string& s) const
{
   try
   {
      n = stoi(s);
      if (n < 0) return false;
      return true;
   }
   catch (...)
   {
      return false;
   }
}

bool App::readDate(Date &date, const string s) const
{
    try
    {
        if (airline.getDateString(s).length() != 8) return false;
        int c = count(s.begin(), s.end(), '/');
        if (c != 2) return false;
        date.setDay(stoi(airline.getDateString(s).substr(0,2)));
        date.setMonth(stoi(airline.getDateString(s).substr(2,2)));
        date.setYear(stoi(airline.getDateString(s).substr(4, 4)));
        if (!date.checkDate(date)) return false;
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool App::readDuration(Date &date, const string s) const
{
    try
    {
        string s2;
        int c = count(s.begin(), s.end(), ':');
        if (c != 1) return false;
        if (s[0] == ':') return false;
        int l = s.length();
        if (s[l-1] == ':') return false;
        int i;
        for (int j  = 0; j < s.length(); j++)
        {
            if (s[j] == ':')
            {
                i = j;
            }
        }
        date.setHour(stoi(s.substr(0, i)));
        date.setMinute(stoi(s.substr(i+1)));
        if (!date.checkDuration(date)) return false;
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool App::readTime(Date &date, const string s) const
{
    try
    {
        if (airline.getTimeString(s).length() != 4) return false;
        int c = count(s.begin(), s.end(), ':');
        if (c != 1) return false;
        date.setHour(stoi(airline.getTimeString(s).substr(0,2)));
        date.setMinute(stoi(airline.getTimeString(s).substr(2, 2)));
        if (!date.checkTime(date)) return false;
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool App::invalidAirportOrigin(string &aO)
{
    while (1) {
        cout << "Airport " << aO << " doesn't exist.\nYou can use menu to go back.\n";
        cout << "Origin airport id: ";
        cin >> aO;
        clearStream();
        if (aO == "menu")
        {
            return false;
        }
        else if (airline.checkAirport(aO)) return true;
    }
}

bool App::invalidAirportDestination(string &aD)
{
    while (1) {
        cout << "Airport " << aD << " doesn't exist.\nYou can use menu to go back.\n";
        cout << "Destination airport id: ";
        cin >> aD;
        clearStream();
        if (aD == "menu")
        {
            return false;
        }
        if (airline.checkAirport(aD)) return true;
    }
}

void App::invalidDepartureDate(Date &date, string &s) const
{
    while(1)
    {
        cout << "Invalid date. Please try again.\n";
        cout << "Departure date (dd/mm/yyyy): ";
        cin >> s;
        clearStream();
        if (readDate(date, s)) break;
    }
}


void App::invalidDepartureTime(Date &date, string &s) const
{
    while(1)
    {
        cout << "Invalid time. Please try again.\n";
        cout << "Departure time (hh:mm): ";
        cin >> s;
        clearStream();
        if (readTime(date, s)) break;
    }
}

void App::invalidDuration(Date &date, string &s) const
{
    while (1)
    {
        cout << "Invalid time. Please try again.\n";
        cout << "Duration time (hh:mm): ";
        cin >> s;
        clearStream();
        if (readDuration(date, s)) break;
    }
}

bool App::invalidPlane(string &id) const
{
    while (1)
    {
        cout << "Plane " << id << " doesn't exist. Try again.\nYou can use menu to go back.\nPlane id: ";
        cin >> id;
        clearStream();
        if (id == "menu") return false;
        if (airline.checkPlane(id)) return true;
    }
}

vector<int> App::findLength(const vector<vector<string>> &table) const
{
   vector<int> res(table[0].size(), 2);
   for (int i = 0; i < table.size(); i++)
   {
      for (int j = 0; j < table[0].size(); j++)
      {
         if (res[j] < table[i][j].size()) res[j] = table[i][j].size();
      }
   }
   return res;
}

void App::displayTable(vector<vector<string> > table, int page) const
{
   vector<int> length = findLength(table);
   vector<string> header = table[0];
   table.erase(table.begin());
   int n_pages = (table.size() + ITEMS_PER_PAGE - 1) / ITEMS_PER_PAGE - 1;
   page = min(n_pages, page - 1);
   page = max(0, page);
   string barrier = "+";
   for (int & i: length) barrier += string(i + 2, '-') + '+';
   cout << barrier << "\n|";
   for (int i = 0; i < header.size(); i++) cout << " " << left << setw(length[i]) << header[i]<< " |";
   cout << endl << barrier << endl;
   for (int i = page * ITEMS_PER_PAGE; i < table.size() && i < (page + 1) * ITEMS_PER_PAGE; i++)
   {
      cout << "|";
      for (int j = 0; j < table[0].size(); j++)
      {
         cout << " " << left << setw(length[j]) << table[i][j] << " |";
      }
      cout << endl;
   }
   cout << barrier << endl;
   string index = "Page " + to_string(page + 1) + "/" + to_string(n_pages + 1);
   cout << right << setw(barrier.size()) << index << endl;
}

bool App::getOption()
{
   char option = 'a';
   while (option != 'y' && option != 'n')
   {
      cin >> option;
      if (cin.eof())
      {
         app_run = false;
         return false;
      }
      option = tolower(option);
      clearStream();
   }
   if (option == 'y') return true;
   return option != 'n';
}

void App::readCommand()
{
   string w;
   getline(cin, w);
   stringstream word(w);
   queue<string> empty;
   swap(command, empty);
   while (getline(word, w, ' '))
   {
      if (!cin)
      {
         if (cin.eof()) app_run = false;
         else cout << "Something went wrong. Please try again.";
      }
      transform(w.begin(), w.end(), w.begin(), [](unsigned char c){return tolower(c);});
      command.push(w);
   }
}

void App::processCommand()
{
   if (!command.empty())
   {
      if (command.front() == "help")
      {
         command.pop();
         help();
         return;
      }
      else if (command.front() == "airport" || command.front() == "a")
      {
         command.pop();
         airport();
         return;
      }
      else if (command.front() == "cart" || command.front() == "c")
      {
         command.pop();
         cart();
         return;
      }
      else if (command.front() == "flight" || command.front() == "f")
      {
         command.pop();
         flight();
         return;
      }
      else if (command.front() == "passenger" || command.front() == "pa")
      {
         command.pop();
         passenger();
         return;
      }
      else if (command.front() == "plane" || command.front() == "pl")
      {
         command.pop();
         plane();
         return;
      }
      else if (command.front() == "ticket" || command.front() == "t")
      {
         command.pop();
         ticket();
         return;
      }
      else if (command.front() == "quit" || command.front() == "q")
      {
         quit();
         return;
      }
   }
   cout << "Invalid command. Verify that it is right or use help.\n";
}


//////////////////////////////////////////////// HELP FUNCTIONS ////////////////////////////////////////////////


void App::help()
{
   if (command.empty())
   {
      cout << "To find out how to use Airline Manager please use the following commands:\n\n";
      cout << "help tutorial\n  - Find out how the program works.\n";
      cout << "help airport\n  - See the airport commands.\n";
      cout << "help cart\n  - See the cart commands.\n";
      cout << "help flight\n  - See the flight commands.\n";
      cout << "help passenger\n  - See the passenger commands.\n";
      cout << "help plane\n  - See the plane commands.\n";
      cout << "help ticket\n  - See the ticket commands.\n\n";
      return;
   }
   else if (command.front() == "tutorial")
   {
      helpTutorial();
      return;
   }
   else if (command.front() == "airport" || command.front() == "a")
   {
      helpAirport();
      return;
   }
   else if (command.front() == "cart" || command.front() == "c")
   {
      helpCart();
      return;
   }
   else if (command.front() == "flight" || command.front() == "f")
   {
      helpFlight();
      return;
   }
   else if (command.front() == "passenger" || command.front() == "pa")
   {
      helpPassenger();
      return;
   }
   else if (command.front() == "plane" || command.front() == "pl")
   {
      helpPlane();
      return;
   }
   else if (command.front() == "ticket" || command.front() == "t")
   {
      helpTicket();
      return;
   }
   cout << "Invalid option. Use help to see the available options.\n";
}

void App::helpTutorial()
{
   cout << "tutorial"; //TODO tutorial
}

void App::helpAirport()
{
   cout << "airport display [page]\n  - Displays airports.\n";
   cout << "airport add 'nameAirport'\n  - Add the airport by name.\n";
   cout << "airport edit name 'nameAirport' \n  - Edits an existing airport name by previous name.\n";
   cout << "airport edit transport add 'nameAirport' \n  - Adds a transport to an existent airport.\n";
   cout << "airport remove 'nameAirport'\n  - Removes an existing airport by name.\n";
   cout << "airport find 'nameAirport'\n  -  Try to locate a airport by name.\n";
}

void App::helpFlight()
{
   cout << "flight display [page]\n  - Displays the existing flights.\n";
   cout << "flight display--p 'startDate' 'endDate' [page]\n  - Displays the existing flights from startDate to endDate.\n";
   cout << "flight add 'id'\n  - Adds the flight by id.\n";
   cout << "flight edit 'id'\n  - Edit an existing flight by id.\n";
   cout << "flight remove 'id'\n  - Removes an existing flight by id.\n";
   cout << "flight find 'id'\n  - Try to locate a flight by id.\n";
   cout << "flight sort 'order\n  - Sorts the flights in the specified order - 'number', 'duration', 'capacity' or 'departure.'\n";
   cout << "flight fly 'id'\n  - Take off flight number id.\n";
   cout << "flight plane 'idFlight' 'idPlane'\n  -  Add plane to flight.\n";
}

void App::helpCart()
{
   cout << "cart display [page]\n  - Displays the existing carts.\n";
   cout << "cart add 'id'\n  - Add the cart by id.\n";
   cout << "cart edit 'id'\n  - Edit the cart by id.\n";
   cout << "cart remove 'id'\n  - Remove a cart by id.\n";
   cout << "cart find 'id'\n  - Try to locate a cart by id.\n";
   cout << "cart assign 'id' 'flight_id'\n  - Assign a cart to a flight.\n";
}

void App::helpPassenger()
{
   cout << "passenger display [page]\n  - Displays the existing passengers.\n";
   cout << "passenger add 'id'\n  - Add the passenger by id.\n";
   cout << "passenger edit 'id'\n  - Edit an existing passenger by id.\n";
   cout << "passenger remove 'id'\n  - Removes an existing passenger by id.\n";
   cout << "passenger find 'id'\n  - Try to locate a passenger by id.\n";
   cout << "passenger search 'name'\n  - Find passengers by name.\n";
   cout << "passenger sort 'order' [page]\n  - Sorts the passengers in the specified order.\n";
   cout << "passenger checkin 'id' 'flight_id'\n  - Check in a passenger to a flight.\n";
}

void App::helpPlane()
{
   cout << "plane display [page]\n  - Displays the existing planes.\n";
   cout << "plane add 'id'\n - Add the plane by id.\n";
   cout << "plane edit 'id'\n  - Edit an existing plane by id.\n";
   cout << "plane remove 'id'\n  - Removes an existing plane by id.\n";
   cout << "plane find 'id'\n  - Try to locate a plane by id.\n";
   cout << "plane sort 'order'\n  - Sorts the plane in the specified order.\n";
   cout << "plane hist 'id' [page]\n  -  Displays plane's flights history.\n";
}

void App::helpTicket()
{
   cout << "ticket buy 'flight_id' 'passenger_id'\n  - Buy the passenger a ticket for the flight.\n";
   cout << "ticket buy 'flight_id' id_list\n  - Buy a ticket to various passengers.\n";
   cout << "ticket display flight 'id'\n  - See who has a ticket to that flight.\n";
   cout << "ticket display passenger 'id'\n  - See the tickets owned by a passenger.\n";
}


//////////////////////////////////////////////// CLASS FUNCTIONS ////////////////////////////////////////////////


void App::airport()
{
    if (command.empty())
    {
        cout << "Invalid command. Use help airport to see available commands.\n";
    }
    else if (command.front() == "add")
    {
        command.pop();
        addAirport();
    }
    else if (command.front() == "display")
    {
        command.pop();
        displayAirport();
    }
    else if (command.front() == "edit")
    {
        command.pop();
        editAirport();
    }
    else if (command.front() == "find")
    {
        command.pop();
        findAirport();
    }
    else if (command.front() == "remove")
    {
        command.pop();
        removeAirport();
    }
    else cout << "Invalid command. Use help airport to get more info.\n";
}

void App::cart()
{
   if (command.empty())
   {
      cout << "Invalid option. Use help cart to see available commands.\n";
   }
   else if (command.front() == "display")
   {
      command.pop();
      displayCart();
   }
   else if (command.front() == "add")
   {
      command.pop();
      addCart();
   }
   else if (command.front() == "edit")
   {
      command.pop();
      editCart();
   }
   else if (command.front() == "remove")
   {
      command.pop();
      removeCart();
   }
   else if (command.front() == "find")
   {
      command.pop();
      findCart();
   }
   else if (command.front() == "assign")
   {
      command.pop();
      assignCart();
   }
   else cout << "Invalid option. Use help cart for more info.\n";
}

void App::flight()
{
   if (command.empty())
   {
       cout << "Invalid option. Use help to see the available commands.\n";
   }
   else if (command.front() == "add")
   {
       command.pop();
       addFlight();
   }
   else if (command.front() == "display")
   {
       command.pop();
       displayFlight();
   }
   else if (command.front() == "display--p")
   {
       command.pop();
       partialDisplayFlight();
   }
   else if (command.front() == "edit")
   {
       command.pop();
       editFlight();
   }
   else if (command.front() == "remove")
   {
       command.pop();
       removeFlight();
   }
   else if (command.front() == "find")
   {
       command.pop();
       findFlight();
   }
   else if (command.front() == "sort")
   {
       command.pop();
       sortFlight();
   }
   else if (command.front() == "fly")
   {
      command.pop();
      flyFlight();
   }
   else if (command.front() == "plane")
   {
       command.pop();
       flightPlane();
   }
   else cout << "Invalid command. Use help flight to get more info.\n";
   return;
}

void App::passenger()
{
   if (command.empty())
   {
      cout << "Invalid command. Use help passenger to see available commands.\n";
   }
   else if (command.front() == "add")
   {
      command.pop();
      addPassenger();
      return;
   }
   else if (command.front() == "display")
   {
      command.pop();
      displayPassenger();
      return;
   }
   else if (command.front() == "edit")
   {
      command.pop();
      editPassenger();
   }
   else if (command.front() == "remove")
   {
      command.pop();
      removePassenger();
   }
   else if (command.front() == "find")
   {
      command.pop();
      findPassenger();
   }
   else if (command.front() == "search")
   {
      command.pop();
      searchPassenger();
   }
   else if (command.front() == "sort")
   {
      command.pop();
      sortPassenger();
   }
   else if (command.front() == "checkin")
   {
      command.pop();
      checkIn();
   }
   else cout << "Invalid command. Use help passenger to get more info.\n";
}

void App::plane()
{
    if (command.empty())
    {
        cout << "Invalid command. Use help passenger to see available commands.\n";
    }
    else if (command.front() == "add")
    {
        command.pop();
        addPlane();
        return;
    }
    else if (command.front() == "display")
    {
        command.pop();
        displayPlane();
        return;
    }
    else if (command.front() == "edit")
    {
        command.pop();
        editPlane();
    }
    else if (command.front() == "remove")
    {
        command.pop();
        removePlane();
    }
    else if (command.front() == "find")
    {
        command.pop();
        findPlane();
    }
    else if (command.front() == "sort")
    {
        command.pop();
        sortPlane();
    }
    else if (command.front() == "hist")
    {
        command.pop();
        historyPlane();
    }
    else cout << "Invalid command. Use help plane to get more info.\n";
}

void App::ticket()
{
    if (command.empty())
    {
        cout << "Command not recognized. Use help ticket to see available commands.\n";
    }
    else if (command.front() == "buy")
    {
        command.pop();
        buyTicket();
    }
    else if (command.front() == "display")
    {
        command.pop();
        displayTicket();
    }
    else cout << "Invalid command. Use help passenger to get more info.\n";
}


//////////////////////////////////////////////// ADD FUNCTIONS ////////////////////////////////////////////////


void App::addAirport()
{
    string name;
    if(command.empty())
    {
        cout << "Usage:\n  airport add 'nameAirport'\n";
        return;
    }
    name = command.front();
    transform(name.begin(),name.end(),name.begin(),::toupper);
    command.pop();
    if (airline.checkAirport(name))
    {
        cout << "That airport already exists. Maybe you want to try:\n  airport edit 'nameAirport'\n";
    }
    else
    {
        airline.addAirport({name});
        cout << "\nAirport " << name << " added to the airline.\n";
    }
}

void App::addCart()
{
   int id, carriages, piles, pile_size;
   string number;
   vector<string> aux = {"Number of carriages: ", "Number of piles: ", "Number of piles: "};
   vector<int> dimensions(3, 0);
   if (command.empty())
   {
      cout << "Usage:\n  cart add 'id'\n";
      return;
   }
   if (!readNumber(id, command.front()))
   {
      cout << "Invalid id. Please try again.\n";
      return;
   }
   command.pop();
   if (airline.checkCart(id))
   {
      cout << "Cart " << id << " already exists. Maybe you want to try:\n  cart edit 'id'\n";
      return;
   }
   for (int i = 0; i < 3; i++)
   {
      do
      {
         cout << aux[i];
         cin >> number;
         clearStream();
      } while(!readNumber(dimensions[i], number));
   }
   airline.addCart({id, dimensions[0], dimensions[1], dimensions[2]});
}

void App::addFlight() {
    int id;
    if (command.empty()) {
        cout << "Usage:\n  flight add 'id'\n";
        return;
    } else if (!readNumber(id, command.front())) {
        cout << "Id must be an integer. Please try again.\n";
        return;
    }
    command.pop();
    if (airline.checkFlight(id)) {
        cout << "That flight already exists. To check existing flights try:\n  flight display\n";
    } else {
        string dD, dT, d, aO, aD, idP; // departureDate, departureTime, duration, airportOrigin, airportDestination, idPlane
        Date departureDate, departureTime, duration;
        if (command.empty())
        {
            cout << "Plane id: ";
            cin >> idP;
            clearStream();
            if (!airline.checkPlane(idP))
            {
                if(!invalidPlane(idP)) return;
            }
        }
        else
        {
            idP = command.front();
            if (!airline.checkPlane(idP))
            {
                if(!invalidPlane(idP)) return;
            }
            command.pop();
        }
        if (command.empty()) {
            cout << "Departure date (dd/mm/yyyy): ";
            cin >> dD;
            clearStream();
            if (!readDate(departureDate, dD)) {
                invalidDepartureDate(departureDate, dD);
            }
        }
        else {
            dD = command.front();
            if (!readDate(departureDate, dD)) {
                invalidDepartureDate(departureDate, dD);
            }
            command.pop();
        }
        if (command.empty()) {
            cout << "Departure time (hh:mm): ";
            cin >> dT;
            clearStream();
            if (!readTime(departureTime, dT)) {
                invalidDepartureTime(departureTime, dT);
            }
        }
        else {
            dT = command.front();
            if (!readTime(departureTime, dT)) {
                invalidDepartureTime(departureTime, dT);
            }
            command.pop();
        }
        if (command.empty()) {
            cout << "Duration (hh:mm): ";
            cin >> d;
            clearStream();
            if (!readDuration(duration, d)) {
                invalidDuration(duration, d);
            }
        }
        else {
            d = command.front();
            if (!readDuration(duration, d)) {
                invalidDuration(duration, d);
            }
            command.pop();
        }
        if (command.empty()) {
            cout << "Origin airport id: ";
            cin >> aO;
            clearStream();
            if (!airline.checkAirport(aO))
            {
                if (!invalidAirportOrigin(aO)) return;
            }
        }
        else
        {
            aO = command.front();
            transform(aO.begin(), aO.end(), aO.begin(), ::toupper);
            if (!airline.checkAirport(aO))
            {
                if (!invalidAirportOrigin(aO)) return;
            }
            command.pop();
        }
        if (command.empty()) {
            cout << "Destination airport id: ";
            cin >> aD;
            clearStream();
            if (!airline.checkAirport(aD))
            {
                if (!invalidAirportDestination(aD)) return;
            }
        }
        else {
            aD = command.front();
            transform(aD.begin(), aD.end(), aD.begin(), ::toupper);
            if (!airline.checkAirport(aD))
            {
                if (!invalidAirportDestination(aD)) return;
            }
            command.pop();
        }
        if (aO == aD)
        {
            cout << "Error. Airport origin and airport destination must be different.\n";
            return;
        }
        airline.addFlight({id, departureDate, departureTime, duration, *airline.findAirport(aO), *airline.findAirport(aD), airline.findPlane(idP)->getCapacity()});
        airline.findFlight(id)->setPlane(idP);
        airline.findPlane(idP)->addFlight({id, departureDate, departureTime, duration, *airline.findAirport(aO), *airline.findAirport(aD), airline.findPlane(idP)->getCapacity()});
        cout << "Flight number " << id << " was added successfully.\n";
    }
}


void App::addPassenger()
{
   int id;
   if (command.empty())
   {
      cout << "Usage:\n  passenger add 'id'\n";
      return;
   }
   if (!readNumber(id, command.front()))
   {
      cout << "Id must be a number. Please try again.\n";
      return;
   }
   command.pop();
   if (airline.checkPassenger(id))
   {
      cout << "That passenger already exists. Maybe you want to try:\n  passenger edit 'id'\n";
   }
   else
   {
      string name;
      cout << "Name: ";
      getline(cin, name);
      airline.addPassenger({id, name});
      cout << "\nPassenger " << name << " added to the airline.\n";
   }

}

void App::addPlane()
{
    if (command.empty())
    {
        cout << "Usage:\n  plane add 'id'\n";
        return;
    }
    transform(command.front().begin(), command.front().end(), command.front().begin(), ::toupper);
    if (command.front() == "NONE")
    {
        cout << "NONE is an invalid plane id. Try again.\n";
        return;
    }
    string id = command.front();
    transform(id.begin(), id.end(), id.begin(), ::toupper);
    command.pop();
    if (airline.checkPlane(id))
    {
        cout << "That plane already exists. To check available planes try:\n  plane display\n";
        return;
    }
    string model, capacity;
    int capacityI;
    if (command.empty())
    {
        cout << "Model: ";
        cin >> model;
        clearStream();
        transform(model.begin(), model.end(), model.begin(), ::toupper);
    }
    else
    {
        model = command.front();
        command.pop();
        transform(model.begin(), model.end(), model.begin(), ::toupper);
    }
    if (command.empty())
    {
        cout << "Capacity: ";
        cin >> capacity;
        clearStream();
        if (!readNumber(capacityI, capacity))
        {
            while (1)
            {
                cout << "Capacity must be an integer. Try again.\n\nCapacity: ";
                cin >> capacity;
                clearStream();
                if (readNumber(capacityI, capacity)) break;
            }
        }
    }
    else
    {
        capacity = command.front();
        if (!readNumber(capacityI, capacity))
        {
            while (1)
            {
                cout << "Capacity must be an integer. Try again.\n\nCapacity: ";
                cin >> capacity;
                clearStream();
                if (readNumber(capacityI, capacity)) break;
            }
        }
        capacityI = stoi(capacity);
    }
    airline.addPlane({id, model, capacityI});
    cout << "Plane " << id << " was added to the airline.\n";
}

//////////////////////////////////////////////// DISPLAY FUNCTIONS ////////////////////////////////////////////////


void App::displayAirport()
{
    int page;
    vector<Airport> airports = airline.getAirports();
    if (airports.size() == 0)
    {
        cout << "No airports to display.\n";
        return;
    }
    vector<vector<string> > table;
    table.push_back({"Name", "Number of Transports"});
    if (command.empty()) page = 0;
    else if (!readNumber(page, command.front()))
    {
        cout << "Page must be a number. Please try again.\n";
        return;
    }
    for (auto & a: airports) table.push_back({a.getName(), to_string(a.getTransports().size())});
    displayTable(table, page);
}

void App::displayCart()
{
   int page;
   vector<Cart> carts = airline.getCarts();
   if (carts.size() == 0)
   {
      cout << "No carts to display.\n";
      return;
   }
   vector<vector<string> > table;
   table.push_back({"Id", "Carriages", "Piles", "Pile Size", "Occupation", "Flight"});
   if (command.empty()) page = 0;
   else if (!readNumber(page, command.front()))
   {
      cout << "Page must be a number. Please try again.\n";
      return;
   }
   for (auto & c: carts)
   {
      string flight = to_string(c.getFlight());
      if (flight == "0") flight = "None";
      table.push_back({to_string(c.getId()), to_string(c.getCarriages()),
                       to_string(c.getPiles()), to_string(c.getPileSize()),
                       to_string(c.getOccupation()) + "%", flight});
   }
   displayTable(table, page);

}

void App::displayFlight()
{
    vector<Flight> flights = airline.getFlights();
    int page;
    if (flights.size() == 0)
    {
        cout << "No flights to display.\n";
        return;
    }
    vector<vector<string>> table;
    table.push_back({"Number", "Departure Date", "Departure Time", "Duration", "Origin Airport", "Destination Airport", "Capacity", "Plane"});
    if (command.empty()) page = 0;
    else if (!readNumber(page, command.front()))
    {
        cout << "Page must be an integer. Please try again.\n";
        return;
    }
    for (auto & f: flights) table.push_back({to_string(f.getNumber()), f.getDepartureDate().displayDate(), f.getDepartureTime().displayTime(), f.getDuration().displayTime(), f.getAirportOrigin().getName(), f.getAirportDestination().getName(), to_string(f.getCapacity()), f.getPlane() });
    displayTable(table, page);
}

void App::partialDisplayFlight()
{
   Date start, end;
   if (command.empty())
   {
       cout << "Invalid command. Try help to see available commands.\n";
       return;
   }
   string since = command.front();
   command.pop();
   if (!readDate(start, since))
   {
       cout << "Invalid date. Please try again.\n";
       return;
   }
   if (command.empty())
   {
       cout << "Invalid command. Try help to see available commands.\n";
       return;
   }
   string until = command.front();
   command.pop();
   if (!readDate(end, until))
   {
       cout << "Invalid date. Please try again.\n";
       return;
   }
   if (end < start)
   {
       cout << "Invalid input. First date has to be older than the second one.\n";
       return;
   }
   int page;
   if (command.empty()) page = 0;
   else if (!readNumber(page, command.front()))
   {
       cout << "Page number must be an integer.\n";
       return;
   }
   vector<Flight> flights;
   vector<vector<string>> table;
   table.push_back({"Number", "Departure Date", "Departure Time", "Duration", "Origin Airport", "Destination Airport", "Capacity", "Plane"});
   bool b = false;
   airline.setFlightOrder("departure");
   int endIndex = airline.getFlights().size() - 1;

   for (auto it: airline.getFlights())
   {
       if (start < it.getDepartureDate() || it.getDepartureDate() == start)
       {
           if (it.getDepartureDate() < end || it.getDepartureDate() == end)
           {
               flights.push_back(it);
           }
       }
   }
   if (flights.size() == 0)
   {
       cout << "There are no flights recorded in that date frame.\n";
       return;
   }
    for (auto & f: flights) table.push_back({to_string(f.getNumber()), f.getDepartureDate().displayDate(), f.getDepartureTime().displayTime(), f.getDuration().displayTime(), f.getAirportOrigin().getName(), f.getAirportDestination().getName(), to_string(f.getCapacity()), f.getPlane()});
    displayTable(table, page);
}

void App::displayPassenger()
{
   int page;
   vector<Passenger> passengers = airline.getPassengers();
   if (passengers.size() == 0)
   {
      cout << "No passengers to display.\n";
      return;
   }
   vector<vector<string>> table;
   table.push_back({"Id", "Name"});
   if (command.empty()) page = 0;
   else if (!readNumber(page, command.front()))
   {
      cout << "Page must be a number. Please try again.\n";
      return;
   }
   for (auto & p: passengers) table.push_back({to_string(p.getId()), p.getName()});
   displayTable(table, page);
}


void App::historyPlane()
{
    if (command.empty())
    {
        cout << "Usage:\n  plane history 'id'\n";
        return;
    }
    transform(command.front().begin(), command.front().end(), command.front().begin(), ::toupper);
    if (!airline.checkPlane(command.front()))
    {
        cout << "Plane " << command.front() << " doesn't exist. Try plane display to see existing planes\n";
        return;
    }
    string idPlane = command.front();
    command.pop();
    int page;
    vector<Flight> flights;
    for (auto &f: airline.findPlane(idPlane)->getFlights())
    {
        flights.push_back(f);
    }
    vector<vector<string>> table;
    table.push_back({"Number", "Departure Date", "Departure Time", "Duration", "Origin Airport", "Destination Airport", "Capacity"});
    if (flights.size() == 0)
    {
        cout << "Plane " << idPlane << " has no flights history. You can try - flight plane 'idFlight' 'idPlane' -  to add flights to a plane.\n";
        return;
    }
    if (command.empty()) page = 0;
    else if (!readNumber(page, command.front()))
    {
        cout << "Page must be an integer. Please try again.\n";
        return;
    }
    for (auto & f: flights) table.push_back({to_string(f.getNumber()), f.getDepartureDate().displayDate(), f.getDepartureTime().displayTime(), f.getDuration().displayTime(), f.getAirportOrigin().getName(), f.getAirportDestination().getName(), to_string(f.getCapacity())});
    displayTable(table, page);
}

void App::displayPlane()
{
    int page;
    vector<Plane> planes = airline.getPlanes();
    if (planes.size() == 0)
    {
        cout << "No planes to display.\n";
        return;
    }
    vector<vector<string>> table;
    table.push_back({"Id", "Model", "Capacity"});
    if (command.empty()) page = 0;
    else if (!readNumber(page, command.front()))
    {
        cout << "Page must be an integer. Please try again.\n";
        return;
    }
    for (auto & p: planes) table.push_back({p.getId(), p.getModel(), to_string(p.getCapacity())});
    displayTable(table, page);
}

void App::displayTicket()
{
   if (command.empty()) {
      cout << "Usage:\n  ";
      cout << "  ticket display flight 'flight_id'\n    - See who has a ticket to that flight.\n";
      cout << "  ticket display passenger 'passenger_id'\n    - See the tickets owned by a passenger.\n";
   }
   else if (command.front() == "flight" || command.front() == "f")
   {
      command.pop();
      displayTicketFlight();
   }
   else if (command.front() == "passenger" || command.front() == "pa")
   {
      command.pop();
      displayTicketPassenger();
   }
   else cout << "Invalid command. Use ticket display to get more info.\n";
}

void App::displayTicketFlight()
{
   int page, flight_id;
   if (command.empty())
   {
      cout << "Usage:\n  - ticket display flight 'flight_id'\n";
      return;
   }
   if (!readNumber(flight_id, command.front()))
   {
      cout << "Please specify a valid Id. Use flight display to see available flights.\n";
      return;
   }
   command.pop();
   if (!airline.checkFlight(flight_id))
   {
      cout << "That flight doesn't exist. Use flight display to see available flights.\n";
      return;
   }
   Flight* flight = airline.findFlight(flight_id);
   vector<PassengerTicket> tickets = airline.getTicketsToFlight(*flight);
   if (tickets.size() == 0)
   {
      cout << "Tickets are yet to be purchased for flight " << flight->getNumber() << ".\n";
      return;
   }
   vector<vector<string> > table;
   table.push_back({"Id", "Name", "Baggage", "Checked in"});
   if (command.empty()) page = 0;
   else if (!readNumber(page, command.front()))
   {
      cout << "Page must be a number. Please try again.\n";
      return;
   }
   for (auto & t: tickets)
   {
      string bag = "No";
      string check_in = "No";
      if (t.ticket.hasCheckedIn()) check_in = "Yes";
      if (t.ticket.getBaggage()) bag = "Yes";
      table.push_back({to_string(t.passenger.getId()),
                       t.passenger.getName(),
                       bag, check_in});
   }
   displayTable(table, page);
}

void App::displayTicketPassenger()
{
   int page, passenger_id;
   if (command.empty())
   {
      cout << "Usage:\n  - ticket display passenger 'passenger_id'\n";
      return;
   }
   if (!readNumber(passenger_id, command.front()))
   {
      cout << "Please specify a valid Id. Use passenger display to see available passengers.\n";
      return;
   }
   command.pop();
   if (!airline.checkPassenger(passenger_id))
   {
      cout << "That passenger doesn't exist. Use passenger display to see available passengers.\n";
      return;
   }
   Passenger* passenger = airline.findPassenger(passenger_id);
   vector<Ticket> tickets = passenger->getTickets();
   if (tickets.size() == 0)
   {
      cout << "Passenger " << passenger->getId() << " doesn't own any tickets yet. Use ticket buy to purchase one.\n";
      return;
   }
   vector<vector<string> > table;
   table.push_back({"Flight Id", "Origin", "Destination", "Baggage", "Checked in"});
   if (command.empty()) page = 0;
   else if (!readNumber(page, command.front()))
   {
      cout << "Page must be a number. Please try again.\n";
      return;
   }
   for (auto & t: tickets) {
      Flight flight = t.getFlight();
      string bag = "No";
      string check_in = "No";
      if (t.hasCheckedIn()) check_in = "Yes";
      if (t.getBaggage()) bag = "Yes";
      table.push_back({to_string(flight.getNumber()),
                       flight.getAirportOrigin().getName(),
                       flight.getAirportDestination().getName(),
                       bag, check_in});
   }
   displayTable(table, page);
}

//////////////////////////////////////////////// EDIT FUNCTIONS ////////////////////////////////////////////////


void App::editAirport()
{
   if (command.empty())
   {
      cout << "Usage:\n  airport edit name 'nameAirport'\n  airport edit transport add 'nameAirport'\n";
      return;
   }
   else if(command.front()=="name")
   {
      command.pop();
      string aname = command.front();
      transform(aname.begin(),aname.end(),aname.begin(),::toupper);
      command.pop();
      if (!airline.checkAirport(aname))
      {
         cout << "That airport doesn't exist. Maybe you want to try:\n  airport add 'nameAirport'\n";
         return;
      }
      Airport a = *airline.findAirport(aname);
      string name;
      cout << "Name: "; cin >> name; clearStream();
      airline.removeAirport(aname);
      a.setName(name);
      airline.addAirport(a);
      cout << "Airport " << name << " was updated.\n";
   }
   else if(command.front()=="transport")
   {
      command.pop();
      if(command.front()=="add")
      {
         command.pop();
         string aname = command.front();
         transform(aname.begin(),aname.end(),aname.begin(),::toupper);
         command.pop();
         if (!airline.checkAirport(aname))
         {
            cout << "That airport doesn't exist. Maybe you want to try:\n  airport add 'nameAirport'\n";
            return;
         }
         Airport a = *airline.findAirport(aname);
         Transport t;
         string distance;
         int i;
         cout << "Type: "; cin >> t.type; clearStream();
         cout << "Distance: "; cin >> distance; clearStream();
         readNumber(t.distance, distance);
         cout << "Number of times: "; cin >> i ; clearStream();
         int n = i;
         vector<Date> v;
         while (i>0)
         {
            string date;
            cout << "Time " << n-i+1 << ": "; cin >> date; clearStream();
            Date d(stoi(airline.getTimeString(date).substr(0,2)),stoi(airline.getTimeString(date).substr(2)));
            v.push_back(d);
            i--;
         }
         t.time = v;
         airline.removeAirport(aname);
         a.addTransport(t);
         airline.addAirport(a);
         cout << "Airport " << aname << " was updated.\n";
      }
   }
   else
   {
      cout << "Usage:\n  airport edit name 'nameAirport'\n  airport edit transport add 'nameAirport'\n";
   }
}

void App::editCart()
{
   int id, carriages, piles, pile_size;
   string number;
   vector<string> aux = {"Number of carriages: ", "Number of piles: ", "Number of piles: "};
   vector<int> dimensions(3, 0);
   if (command.empty())
   {
      cout << "Usage:\n  cart edit 'id'\n";
      return;
   }
   if (!readNumber(id, command.front()))
   {
      cout << "Invalid id. Please try again.\n";
      return;
   }
   command.pop();
   if (!airline.removeCart(id))
   {
      cout << "Cart " << id << " does not exist. Adding a new one.\n";
   }
   for (int i = 0; i < 3; i++)
   {
      do
      {
         cout << aux[i];
         cin >> number;
         clearStream();
      } while(!readNumber(dimensions[i], number));
   }
   airline.addCart({id, dimensions[0], dimensions[1], dimensions[2]});
}

void App::editFlight()
{
    int id;
    string attribute;
    if (command.empty()) {
        cout << "Usage:\n  flight edit 'id'\n";
        return;
    }
    if (!readNumber(id, command.front())) {
        cout << "Id must be an integer. Please try again.\n";
        return;
    }
    if (!airline.checkFlight(stoi(command.front())))
    {
        cout << "That flight doesn't exist. To check existing flights try:\n  flight display\n";
        return;
    }
    id = stoi(command.front());
    command.pop();
    Date departureDate, departureTime, duration;
    if (command.empty())
    {
        cout << "Which attribute do you want to edit?\nOptions: departureDate, departureTime, duration, origin, destination, capacity, plane.\nAttribute: ";
        cin >> attribute;
        clearStream();
        if (attribute == "departureDate")
        {
            string dD;
            cout << "New departure date (dd/mm/yyyy): ";
            cin >> dD;
            clearStream();
            if (!readDate(departureDate, dD)) {
                invalidDepartureDate(departureDate, dD);
            }
            Flight f(id, departureDate, airline.findFlight(id)->getDepartureTime(), airline.findFlight(id)->getDuration(), airline.findFlight(id)->getAirportOrigin(), airline.findFlight(id)->getAirportDestination(), airline.findFlight(id)->getCapacity());
            f.setPlane(airline.findFlight(id)->getPlane());
            airline.findPlane(airline.findFlight(id)->getPlane())->removeFlight(*airline.findFlight(id));
            airline.findPlane(airline.findFlight(id)->getPlane())->addFlight(f);
            airline.removeFlight(id);
            airline.addFlight(f);
            cout << "Flight " << id << " was updated with a new departure date.\n";
            return;
        }
         if (attribute == "departureTime")
        {
            string dT;
            cout << "New departure time (hh:mm): ";
            cin >> dT;
            clearStream();
            if (!readDate(departureTime, dT)) {
                invalidDepartureTime(departureTime, dT);
            }
            Flight f(id, airline.findFlight(id)->getDepartureDate(), departureTime, airline.findFlight(id)->getDuration(), airline.findFlight(id)->getAirportOrigin(), airline.findFlight(id)->getAirportDestination(), airline.findFlight(id)->getCapacity());
            f.setPlane(airline.findFlight(id)->getPlane());
            airline.findPlane(airline.findFlight(id)->getPlane())->removeFlight(*airline.findFlight(id));
            airline.findPlane(airline.findFlight(id)->getPlane())->addFlight(f);
            airline.removeFlight(id);
            airline.addFlight(f);
            cout << "Flight " << id << " was updated with a new departure time.\n";
            return;
        }
         if (attribute == "duration")
        {
            string d;
            cout << "New duration (hh:mm): ";
            cin >> d;
            clearStream();
            if (!readDuration(duration, d)) {
                invalidDuration(duration, d);
            }
            Flight f(id, airline.findFlight(id)->getDepartureDate(), airline.findFlight(id)->getDepartureTime(), duration, airline.findFlight(id)->getAirportOrigin(), airline.findFlight(id)->getAirportDestination(), airline.findFlight(id)->getCapacity());
            f.setPlane(airline.findFlight(id)->getPlane());
            airline.findPlane(airline.findFlight(id)->getPlane())->removeFlight(*airline.findFlight(id));
            airline.findPlane(airline.findFlight(id)->getPlane())->addFlight(f);
            airline.removeFlight(id);
            airline.addFlight(f);
            cout << "Flight " << id << " was updated with a new duration.\n";
            return;
        }
         if (attribute == "origin")
        {
            string aO;
            cout << "New origin airport id: ";
            cin >> aO;
            clearStream();
            transform(aO.begin(), aO.end(), aO.begin(), ::toupper);
            if (!airline.checkAirport(aO))
            {
                if (!invalidAirportOrigin(aO)) return;
            }
            Flight f(id, airline.findFlight(id)->getDepartureDate(), airline.findFlight(id)->getDepartureTime(), airline.findFlight(id)->getDuration(), *airline.findAirport(aO), airline.findFlight(id)->getAirportDestination(), airline.findFlight(id)->getCapacity());
            f.setPlane(airline.findFlight(id)->getPlane());
            airline.findPlane(airline.findFlight(id)->getPlane())->removeFlight(*airline.findFlight(id));
            airline.findPlane(airline.findFlight(id)->getPlane())->addFlight(f);
            airline.removeFlight(id);
            airline.addFlight(f);
            cout << "Flight " << id << " was updated with a new origin airport.\n";
            return;
        }
         if (attribute == "destination")
        {
            string aD;
            cout << "New destination airport id: ";
            cin >> aD;
            clearStream();
            transform(aD.begin(), aD.end(), aD.begin(), ::toupper);
            if (!airline.checkAirport(aD))
            {
                if (!invalidAirportDestination(aD)) return;
            }
            Flight f(id, airline.findFlight(id)->getDepartureDate(), airline.findFlight(id)->getDepartureTime(), airline.findFlight(id)->getDuration(), *airline.findAirport(aD), airline.findFlight(id)->getAirportDestination(), airline.findFlight(id)->getCapacity());
            f.setPlane(airline.findFlight(id)->getPlane());
            airline.findPlane(airline.findFlight(id)->getPlane())->removeFlight(*airline.findFlight(id));
            airline.findPlane(airline.findFlight(id)->getPlane())->addFlight(f);
            airline.removeFlight(id);
            airline.addFlight(f);
            cout << "Flight " << id << " was updated with a new destination airport.\n";
            return;
        }
         if (attribute == "capacity")
        {
            string capacity;
            int capacityI;
            cout << "New flight capacity: ";
            cin >> capacity;
            clearStream();
            if (!readNumber(capacityI, capacity))
            {
                while (1) {
                    cout << "Flight capacity must be an integer. Please try again.\nFlight capacity: ";
                    cin >> capacity;
                    clearStream();
                    if (readNumber(capacityI, capacity)) break;
                }
            }
            Flight f(id, airline.findFlight(id)->getDepartureDate(), airline.findFlight(id)->getDepartureTime(), airline.findFlight(id)->getDuration(), airline.findFlight(id)->getAirportOrigin(), airline.findFlight(id)->getAirportDestination(), capacityI);
            f.setPlane(airline.findFlight(id)->getPlane());
            if (airline.findFlight(id)->getPlane() != "NONE")
            {
                airline.findPlane(airline.findFlight(id)->getPlane())->removeFlight(*airline.findFlight(id));
                airline.findPlane(airline.findFlight(id)->getPlane())->addFlight(f);
            }
            airline.removeFlight(id);
            airline.addFlight(f);
            cout << "Flight " << id << " was updated with a new capacity.\n";
            return;
        }
         if (attribute == "plane")
         {
             string idP;
             cout << "New plane: ";
             cin >> idP;
             clearStream();
             if (!airline.checkPlane(idP))
             {
                 cout << "Plane " << idP << " doesn't exist. Try plane display to check existing planes.\n";
                 return;
             }
             if (airline.findFlight(id)->getPlane() == "NONE")
             {
                 Flight f(id, airline.findFlight(id)->getDepartureDate(), airline.findFlight(id)->getDepartureTime(), airline.findFlight(id)->getDuration(), airline.findFlight(id)->getAirportOrigin(), airline.findFlight(id)->getAirportDestination(), airline.findFlight(id)->getCapacity());
                 f.setPlane(idP);
                 airline.findPlane(idP)->addFlight(f);
                 airline.removeFlight(id);
                 airline.addFlight(f);
                 cout << "Flight " << id << " was updated with a new plane.\n";
                 return;
             }
             Flight f(id, airline.findFlight(id)->getDepartureDate(), airline.findFlight(id)->getDepartureTime(), airline.findFlight(id)->getDuration(), airline.findFlight(id)->getAirportOrigin(), airline.findFlight(id)->getAirportDestination(), airline.findFlight(id)->getCapacity());
             f.setPlane(idP);
             airline.findPlane(airline.findFlight(id)->getPlane())->removeFlight(*airline.findFlight(id));  // removes flight from old plane's flights list
             airline.findPlane(idP)->addFlight(f);  // adds flight to new plane's flights list
             airline.removeFlight(id);
             airline.addFlight(f);
             cout << "Flight " << id << " was updated with a new plane.\n";
             return;
         }
        cout << "Invalid option.\n";
        return;
    }
    else
    {
        cout << "Invalid command. Use help to see the available commands.\n";
        return;
    }
}

void App::editPassenger()
{
   int id;
   string name;
   if (command.empty())
   {
      cout << "Usage:\n  passenger edit 'id'\n";
      return;
   }
   if (!readNumber(id, command.front()))
   {
      cout << "Id must be a number. Please try again.\n";
      return;
   }
   if (!airline.removePassenger(id))
   {
      cout << "That passenger doesn't exist. Maybe you want to try:\n  passenger add 'id'\n";
      return;
   }
   cout << "Name: "; cin >> name; clearStream();
   airline.addPassenger({id, name});
   cout << "Passenger " << name << " was updated.\n";
}

void App::editPlane()
{
    if (command.empty())
    {
        cout << "Usage:\n  plane edit 'id'.\n";
        return;
    }
    if (!airline.checkPlane(command.front()))
    {
        cout << "That plane doesn't exist. To check existing planes try:\n  plane display\n";
        return;
    }
    string id = command.front();
    command.pop();
    if (command.empty())
    {
        string attribute;
        cout << "Which attribute do you want to edit?\nOptions: model, capacity\nAttribute: ";
        cin >> attribute;
        clearStream();
        if (attribute == "model")
        {
            string model;
            cout << "New model: ";
            cin >> model;
            clearStream();
            Plane p(id, model, airline.findPlane(id)->getCapacity());
            airline.removePlane(id);
            airline.addPlane(p);
            cout << "Plane " << id << " was updated with a new model name.\n";
            return;
        }
        if (attribute == "capacity")
        {
            string capacity;
            int capacityI;
            cout << "New capacity: ";
            cin >> capacity;
            clearStream();
            if (!readNumber(capacityI, capacity))
            {
                while (1)
                {
                    cout << "Capacity must be an integer. Try again.\nCapacity: ";
                    cin >> capacity;
                    clearStream();
                    if (readNumber(capacityI, capacity)) break;
                }
            }
            Plane p(id, airline.findPlane(id)->getModel(), capacityI );
            airline.removePlane(id);
            airline.addPlane(p);
            cout << "Plane " << id << " was updated with a new capacity.\n";
            return;
        }
        cout << "Invalid attribute.\n";
        return;
    }
    else
    {
        cout << "Invalid command. Try help to see available commands.\n";
        return;
    }
}

//////////////////////////////////////////////// REMOVE FUNCTIONS ////////////////////////////////////////////////


void App::removeAirport()
{
   string name;
   if (command.empty())
   {
      cout << "Usage:\n  airport remove 'nameAirport'\n";
      return;
   }
   name = command.front();
   transform(name.begin(),name.end(),name.begin(),::toupper);
   if (airline.removeAirport(name))
   {
      cout << "Airport " << name << " was removed.\n";
   }
   else cout << "That Airport doesn't exist.\n";
}

void App::removeFlight()
{
    int id;
    if (command.empty())
    {
        cout << "Usage:\n  flight remove 'id'\n";
        return;
    }
    if (!readNumber(id, command.front()))
    {
        cout << "Id must be an integer. Please try again.\n";
        return;
    }
    if (airline.removeFlight(id))
    {
        cout << "Flight " << id << " was removed.\n";
    }
    else cout << "That flight doesn't exist.\n";
}

void App::removeCart()
{
   int id;
   if (command.empty())
   {
      cout << "Usage:\n  cart remove 'id'\n";
      return;
   }
   if (!readNumber(id, command.front()))
   {
      cout << "Invalid id. Please try again.\n";
      return;
   }
   if (airline.removeCart(id))
   {
      cout << "Cart " << id << " was removed.\n";
   }
   else cout << "That cart doesn't exist.\n";
}

void App::removePassenger()
{
   int id;
   if (command.empty())
   {
      cout << "Usage:\n  passenger remove 'id'\n";
      return;
   }
   if (!readNumber(id, command.front()))
   {
      cout << "Id must be a number. Please try again.\n";
      return;
   }
   if (airline.removePassenger(id))
   {
      cout << "Passenger " << id << " was removed.\n";
   }
   else cout << "That passenger doesn't exist.\n";
}

void App::removePlane()
{
    if (command.empty())
    {
        cout << "Usage:\n  plane remove 'id'\n";
        return;
    }
    string idP = command.front();
    transform(idP.begin(), idP.end(), idP.begin(), ::toupper);
    command.pop();
    if (!command.empty())
    {
        cout << "Invalid command. Try again.\n";
        return;
    }
    if (airline.removePlane(idP))
    {
        cout << "Plane " << idP << " was removed.\n";
    }
    else cout << "That plane doesn't exist.\n";
}


//////////////////////////////////////////////// FIND FUNCTIONS ////////////////////////////////////////////////


void App::findAirport()
{
   int page;
   string name;
   if (command.empty())
   {
      cout << "Usage:\n  - airport find 'nameAirport'\n";
      return;
   }
   name = command.front();
   command.pop();
   transform(name.begin(),name.end(),name.begin(),::toupper);
   if (airline.checkAirport(name))
   {
      Airport* a = airline.findAirport(name);
      if (command.empty()) page = 0;
      else if (!readNumber(page, command.front()))
      {
          cout << "Page must be a number. Please try again.\n";
          return;
      }
      cout << "Name: " << a->getName() << " \nNumber of Transports: " << a->getTransports().size() << " \n";
      if(a->getTransports().size()!=0) {
         cout << "Closest Transport: " << a->findClosest().type << " at " << a->findClosest().distance << " kilometers" << "\n";
         vector<vector<string> > table;
         table.push_back({"Type", "Distance", "First time", "Last time"});
         for (auto b: a->getTransports() ) table.push_back({b.type, to_string(b.distance), b.time.begin()->displayTime(), (b.time.end()-1)->displayTime()});
         displayTable(table, page);
      }
   }
   else cout << "Airport not found.\n";
}

void App::findFlight()
{
   int id;
   if (command.empty())
   {
        cout << "Usage:\n  - flight find 'id'\n";
        return;
   }
   if (!readNumber(id, command.front()))
   {
       cout << "Id must be an integer. Please try again.\n";
       return;
   }
   if (airline.checkFlight(id))
   {
       Flight* f = airline.findFlight(id);
       cout << "Id: " << f->getNumber() << "\nDeparture date: " << f->getDepartureDate().displayDate() << "\nDeparture time: " << f->getDepartureTime().displayDate() << "\nDuration: " << f->getDuration().displayTime() << "\nOrigin airport: " << f->getAirportOrigin().getName() << "\nDestination airport: " << f->getAirportDestination().getName() << "\nCapacity: " << f->getCapacity() << endl;
   }
   else cout << "Flight not found. To check existing flights you can try: \n  flight display\n";
}

void App::findCart()
{
   int id;
   if (command.empty())
   {
      cout << "Usage:\n  cart find 'id'\n";
      return;
   }
   if (!readNumber(id, command.front()))
   {
      cout << "Invalid id. Please try again.\n";
      return;
   }
   if (airline.checkCart(id))
   {
      Cart* cart = airline.findCart(id);
      cout << "Id: " << cart->getId() << "\nCarriages. " << cart->getCarriages();
      cout << "\nNumber of piles: " << cart->getPiles() << "\nBags per pile: ";
      cout << cart->getPileSize() << "\nAssigned flight: ";
      if (cart->getFlight() == 0) cout << "None\n";
      else cout << cart->getFlight() << '\n';
   }
   else cout << "Cart not found.\n";
}

void App::findPassenger()
{
   int id;
   if (command.empty())
   {
      cout << "Usage:\n  - passenger find 'id'\n";
      return;
   }
   if (!readNumber(id, command.front()))
   {
      cout << "Id must be a number. Please try again.\n";
      return;
   }
   if (airline.checkPassenger(id))
   {
      Passenger* p = airline.findPassenger(id);
      cout << "Id: " << p->getId() << " \nName: " << p->getName() << "\n";
   }
   else cout << "Passenger not found.\n";
}

void App::searchPassenger()
{
   int page = 0;
   string name;
   vector<Passenger> passengers;
   vector<vector<string> > table;
   if (command.empty())
   {
      cout << "Usage:\n  - passenger search 'name' [page]\n";
      return;
   }
   while (!readNumber(page, command.front()))
   {
      name += command.front();
      command.pop();
      if (command.empty()) break;
   }
   passengers = airline.searchNames(name);
   if (passengers.empty())
      cout << "No passengers match that name.\n";
   else
   {
      table.push_back({"Id", "Name"});
      for (auto & p: passengers) table.push_back({to_string(p.getId()), p.getName()});
      displayTable(table, page);
   }
}

void App::findPlane()
{

    if (command.empty()){
        cout << "usage:\n  - plane find 'id'\n";
        return;
    }
    if (airline.checkPlane(command.front()))
    {
        Plane* p = airline.findPlane(command.front());
        cout << "Id: " << p->getId() << "\nModel: " << p->getModel() << "\nCapacity: " << p->getCapacity() << endl;
    }
    else cout << "Plane not found.\n";
}

//////////////////////////////////////////////// SORT FUNCTIONS ////////////////////////////////////////////////


void App::sortFlight()
{
    if (!command.empty())
    {
        if (command.front() == "number" || command.front() == "departure" || command.front() == "duration" || command.front() == "capacity")
        {
            airline.setFlightOrder(command.front());
            cout << "Flights sorted by " << command.front() << ".\n";
        }
        else
        {
            cout << "Invalid sort option. Use help flight for more info.\n";
        }
    }
    else
    {
        cout << "Usage:\n  flight sort 'order'\n\n  Order options: number, departure, duration, capacity\n";
    }
}

void App::sortPassenger()
{
   if (!command.empty()) {
      if (command.front() == "id" || command.front() == "name") {
         airline.setPassengerOrder(command.front());
         cout << "Passengers sorted by " << command.front() << ".\n";
      }
   }
   else
   {
      cout << "Usage:\n  passenger sort 'order'\n\n  Order options: id, name\n";
   }
}

void App::sortPlane()
{
    if (!command.empty())
    {
        if (command.front() == "id" || command.front() == "model" || command.front() == "capacity")
        {
            airline.setPlaneOrder(command.front());
            cout << "Planes sorted by " << command.front() << "\n";
        }
        else
        {
            cout << "Invalid sort option. Try plane help for more info.\n";
        }
    }
    else
    {
        cout << "Usage:\n  plane sort 'order'\n\n  Order option: id, model, capacity\n";
    }
}

void App::buyTicket()
{
   int flight_id, passenger_id;
   vector<GroupMember> group;
   if (command.empty())
   {
      cout << "Usage:\n";
      cout << "  ticket buy 'flight_id' 'passenger_id'\n    - Buy the passenger a ticket for the flight.\n";
      cout << "  ticket buy 'flight_id' id_list\n    - Buy a ticket to various passengers.\n";
      return;
   }
   if (!readNumber(flight_id, command.front()))
   {
      cout << "Invalid flight. Use flight display to see available flights.\n";
      return;
   }
   Flight* flight = airline.findFlight(flight_id);
   if (!airline.checkFlight(flight_id))
   {
      cout << "Flight not recognized. Use flight add to add a flight or flight display to see available flights.\n";
      return;
   }
   command.pop();
   while (!command.empty())
   {
      if (!readNumber(passenger_id, command.front()))
      {
         cout << "'" << command.front() << "' is not a valid passenger. Use passenger display to see available passengers.\n";
         return;
      }
      if (!airline.checkPassenger(passenger_id))
      {
         cout << "Passenger " << passenger_id << " not found. Use passenger help to add a passenger or passenger display to see the available passengers.\n";
         return;
      }
      Passenger* pa = airline.findPassenger(passenger_id);
      cout << "Include baggage for passenger " << pa->getName() << " (" << pa->getId() << ")? (y/n):";
      group.push_back({pa, getOption()});
      command.pop();
   }
   if (airline.buyTicket(flight, group))
   {
      cout << "\nThe tickets to " << flight->getAirportDestination().getName() << " were bought.\n";
   }
   else
      cout << "Couldn't buy tickets. Please try again.\n";
}

void App::checkIn()
{
   int id, flight_id;
   if (command.empty())
   {
      cout << "Usage:\n  passenger checkin 'id' 'flight_id'\n";
      return;
   }
   if (!readNumber(id, command.front()))
   {
      cout << "Not a valid Passenger id. Please try again.\n";
      return;
   }
   command.pop();
   if (!readNumber(flight_id, command.front()))
   {
      cout << "Not a valid Flight id. Please try again.\n";
      return;
   }
   command.pop();
   if (!airline.checkPassenger(id))
   {
      cout << "Passenger not found. Use passenger display to see available passengers.\n";
      return;
   }
   if (!airline.checkFlight(flight_id))
   {
      cout << "Flight not found. Please check the flight Id.\n";
      return;
   }
   Passenger* passenger = airline.findPassenger(id);
   Flight* flight = airline.findFlight(flight_id);
   if (passenger->ticketOwned(*flight))
   {
      Ticket ticket = passenger->getTicket(*flight);
      if (ticket.hasCheckedIn())
      {
         cout << "Passenger " << id << " has already checked in to that flight.\n";
         return;
      }
      if (!passenger->getTicket(*flight).getBaggage())
      {
         airline.checkIn(flight_id, id);
      }
      else
      {
         double weight;
         cout << "Baggage weight (kg): ";
         cin >> weight;
         clearStream();
         if (!airline.checkIn(flight_id, id, weight))
         {
            cout << "Flight " << flight_id << "'s carts are all full. Try adding a new one before checking in a passenger.\n";
            return;
         }
      }
      cout << "Check in successful!\n";
   }
}

void App::assignCart()
{
   int id, flight_id;
   if (command.empty())
   {
      cout << "Usage:\n  cart assign 'id' 'flight_id'\n";
      return;
   }
   if (!readNumber(id, command.front()))
   {
      cout << "Invalid id. Please try again.\n";
      return;
   }
   if (!airline.checkCart(id))
   {
      cout << "That cart doesn't exist. Use cart display to see available carts.\n";
      return;
   }
   command.pop();
   if (!readNumber(flight_id, command.front()))
   {
      cout << "Invalid id. Please try again.\n";
      return;
   }
   if (!airline.checkFlight(flight_id))
   {
       cout << "That flight doesn't exist. Use flight display to see available flights.\n";
       return;
   }
   Cart* cart = airline.findCart(id);
   if (cart->getFlight() != 0)
   {
      cout << "Cart " << id << " is already assigned to a flight.\n";
   }
   else
   {
      airline.assignCartFlight(id, flight_id);
      cout << "Cart " << id << " was assigned to flight " << flight_id << ".\n";
   }
}

void App::flyFlight()
{
   int id, n_tickets, n_passengers, n_seats;
   if (command.empty())
   {
      cout << "Usage:\n  flight fly 'id'\n";
      return;
   }
   if (!readNumber(id, command.front()))
   {
      cout << "Invalid id. Please try again.\n";
      return;
   }
   if (!airline.checkFlight(id))
   {
       cout << "That flight doesn't exist. Use flight display to see available flights.\n";
       return;
   }
   Flight* flight = airline.findFlight(id);
   n_seats = flight->getCapacity();
   n_passengers = airline.getCheckedIn(id);
   n_tickets =  airline.flyFlight(id);
   cout << "The flight took of with " << n_passengers << " passengers and " << n_tickets << "/" << n_seats << " tickets sold.\n";
}

void App::flightPlane()
{
    if (command.empty())
    {
        cout << "Usage:\n  flight plane 'idFlight' 'idPlane'\n";
        return;
    }
    int idF;
    string idP;
    if (!readNumber(idF, command.front()))
    {
        cout << "Flight id must me an integer.\n";
        return;
    }
    if (!airline.checkFlight(idF))
    {
        cout << "Flight number " << idF << " doesn't exist. Use flight display to check existing flights.\n";
        return;
    }
    command.pop();
    idP = command.front();
    transform(idP.begin(), idP.end(), idP.begin(), ::toupper);
    if (!airline.checkPlane(idP))
    {
        cout << "Plane number " << idP << " doesn't exist. Use plane display to check existing planes.\n ";
        return;
    }
    airline.findFlight(idF)->setPlane(idP);
    airline.findPlane(idP)->addFlight(*airline.findFlight(idF));
    cout << "Flight number " << idF << " was successfully attributed to plane " << idP << ".\n";
}

void App::quit()
{
   app_run = false;
}