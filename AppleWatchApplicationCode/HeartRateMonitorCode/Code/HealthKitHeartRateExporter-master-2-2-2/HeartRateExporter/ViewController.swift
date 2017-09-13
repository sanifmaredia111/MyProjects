

import UIKit
import HealthKit
import FirebaseDatabase
import Firebase
import AWSDynamoDB
import AWSSQS
import AWSSNS
import AWSCognito
import AWSS3



class ViewController: UIViewController{
    //var ref:FIRDatabaseReference?
   
    var csvString = "";
    var dt = [String]();
    let healthStore = HKHealthStore()
    let heartRateType = HKQuantityType.quantityType(forIdentifier: HKQuantityTypeIdentifier.heartRate)!
    var hrdataArray = [String]();
    var hrtimeArray = [String]();
    var hrdateArray = [String]();
    var intarr = [Int]();
    var finaldttime = "";
    var timer = Timer();
 
    var dynamoDBObjectMapper : AWSDynamoDBObjectMapper? = nil;
    
    let myheartrate =  Heartratemonitor()

    
    @IBOutlet weak var dtlbl: UILabel!
    
    @IBOutlet weak var hrlbl: UILabel!
    
    @IBOutlet weak var timelbl: UILabel!
    
    
    @IBOutlet weak var savedata: UIButton!
 
   /* @IBAction func savedatatocsv(_ sender: Any) {
        
       // let ref = FIRDatabase.database().reference()
        //ref.child("data").child("heart rate").childByAutoId().setValue(hrdataArray + hrdateArray + hrtimeArray)
        //ref.child("data").child("date").childByAutoId().setValue(hrdateArray)
        //ref.child("data").child("time").childByAutoId().setValue(hrtimeArray)

        
        let alert = UIAlertController(title: "Save", message: "Your data is saved successfully.", preferredStyle: UIAlertControllerStyle.alert)
        
        // add an action (button)
        alert.addAction(UIAlertAction(title: "OK", style: UIAlertActionStyle.default, handler: nil))
        
        // show the alert
        self.present(alert, animated: true, completion: nil)
    }
    */
 
   
    
    
    
    @IBAction func stopbtn(_ sender: Any) {
        timer.invalidate()
    }
    
    
    
    
      @IBAction func playbtn(_ sender: Any) {
       
        if (HKHealthStore.isHealthDataAvailable()){
            timer = Timer.scheduledTimer(timeInterval: 10.0, target: self, selector: #selector(ViewController.calculate), userInfo: nil, repeats: true)
    }
    
    }
    
  override func viewDidLoad() {
    
        super.viewDidLoad()

    
    
   
    }
    
    
    override func viewWillAppear(_ animated: Bool) {
        //Save data to AWSDynamoDB
       dynamoDBObjectMapper = AWSDynamoDBObjectMapper.default();

       /* self.myheartrate?.Heartrate = "heart rate1"
        self.myheartrate?.Date = "date1"
        self.myheartrate?.Time = "time1"
        
        self.dynamoDBObjectMapper?.save(self.myheartrate!).continueWith(block: { (task:AWSTask<AnyObject>!) -> Any? in
            if let error = task.error as? NSError {
                print("The request failed. Error: \(error)")
            } else {
                // Do something with task.result or perform other operations.
            }
            return nil
        })
      */  

    }
    //Get Data from Health Store
    func calculate(){
      
        var hrrate = "";
        var  hrdate = "";
        var hrtime = "";
        var result = "";
        //var csvString = "";
       // let healthStore = HKHealthStore()
      
      // if (HKHealthStore.isHealthDataAvailable()){
        csvString = "Time,Date,Heartrate(BPM)\n"
            healthStore.requestAuthorization(toShare: nil, read:[heartRateType], completion:{(success, error) in

                let sortByTime = NSSortDescriptor(key:HKSampleSortIdentifierEndDate, ascending:false)
                let timeFormatter = DateFormatter()
                timeFormatter.dateFormat = "hh:mm:ss"
                
                let dateFormatter = DateFormatter()
                dateFormatter.dateFormat = "MM/dd/YYYY"
                let query = HKSampleQuery(sampleType:self.heartRateType, predicate:nil, limit:1, sortDescriptors:[sortByTime], resultsHandler:{(query, results, error) in
                    guard let results = results else { return }
                    for quantitySample in results {
                        let quantity = (quantitySample as! HKQuantitySample).quantity
                        let heartRateUnit = HKUnit(from: "count/min")
                        self.csvString += "\(timeFormatter.string(from: quantitySample.startDate)),\(dateFormatter.string(from: quantitySample.startDate)),\(quantity.doubleValue(for: heartRateUnit))\n"
                            print("\(timeFormatter.string(from: quantitySample.startDate)),\(dateFormatter.string(from: quantitySample.startDate)),\(quantity.doubleValue(for: heartRateUnit))")
                        hrdate =  dateFormatter.string(from: quantitySample.startDate);
                        hrtime = timeFormatter.string(from: quantitySample.startDate);
                        self.hrlbl.text = String(quantity.doubleValue(for: heartRateUnit))
                        self.dtlbl.text = hrdate;
                        self.timelbl.text = hrtime;
                        hrrate = self.hrlbl.text!;
                        result = hrrate + "_" + hrdate + "_" + hrtime;
                        //Save data to firebase
                        let ref = FIRDatabase.database().reference()
                        ref.childByAutoId().setValue(result);
                        self.finaldttime = hrdate + " : " ;
                        self.finaldttime += hrtime;
                        
                        //passing data to dynamodb tablerows
                       // self.dynamoDBObjectMapper = AWSDynamoDBObjectMapper.default();
                        
                        self.myheartrate?.Heartrate = result
                        self.myheartrate?.Date = hrdate
                        self.myheartrate?.Time = hrtime
                        
                        self.dynamoDBObjectMapper?.save(self.myheartrate!).continueWith(block: { (task:AWSTask<AnyObject>!) -> Any? in
                            if let error = task.error as? NSError {
                                print("The request failed. Error: \(error)")
                            } else {
                                // Do something with task.result or perform other operations.
                            }
                            return nil
                        })

                                               self.hrdataArray.append(String(quantity.doubleValue(for: heartRateUnit)));
                        self.hrtimeArray.append(timeFormatter.string(from: quantitySample.startDate));
                        self.hrdateArray.append(dateFormatter.string(from: quantitySample.startDate));
                        self.dt.append(self.finaldttime);
                        print(self.hrdateArray);
                        
                            

                    }
                    let size = self.hrdataArray.count;
                    print(size);
                  /*  self.hrlbl.text = self.hrdataArray.first;
                
                    self.dtlbl.text = self.hrdateArray.first;
                    
                    self.timelbl.text = self.hrtimeArray.first;
                    */
                   // self.hrlbl.text = String(quantity.doubleValue(for: heartRateUnit));
                    
                   

                })
                self.healthStore.execute(query)
               
            })
        }
    
    
  
}

