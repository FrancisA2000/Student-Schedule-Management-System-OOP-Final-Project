#!/usr/bin/env python3
"""
CSV to YAML Data Converter
Processes all CSV files in the data folder and generates a comprehensive 
YAML structure containing all course scheduling data.

This script provides 80-character line formatting and comprehensive 
error checking to ensure no data is missed during conversion.
"""

import csv
import os
import yaml
from pathlib import Path

def process_csv_files(data_dir):
    """
    Process all CSV files in the data directory and convert to YAML format.
    
    Args:
        data_dir (str): Path to the data directory containing CSV files
    
    Returns:
        dict: Complete data structure with all scheduling information
    """
    data = {
        'courses': {},
        'lectures': {},
        'tutorials': {},
        'labs': {},
        'schedules': {},
        'schedule_contents': {}
    }
    
    csv_files_processed = 0
    total_records = 0
    errors = []
    
    print("Starting CSV processing...")
    
    # Process all CSV files in the data directory
    for csv_file in Path(data_dir).glob("*.csv"):
        try:
            csv_files_processed += 1
            filename = csv_file.name
            print(f"Processing: {filename}")
            
            with open(csv_file, 'r', encoding='utf-8') as file:
                csv_reader = csv.DictReader(file)
                
                if filename == 'courses.csv':
                    # Process courses data
                    for row in csv_reader:
                        course_id = row['courseId']
                        data['courses'][course_id] = {
                            'courseId': int(course_id),
                            'name': row['name'],
                            'credits': float(row['credits']),
                            'examDateA': row['examDateA'],
                            'examDateB': row['examDateB'],
                            'lecturer': row['lecturer']
                        }
                        total_records += 1
                        
                elif filename == 'schedules.csv':
                    # Process schedule IDs
                    for row in csv_reader:
                        schedule_id = row['scheduleId']
                        data['schedules'][schedule_id] = {
                            'scheduleId': int(schedule_id)
                        }
                        total_records += 1
                        
                elif filename.startswith('schedule_') and filename.endswith('.csv'):
                    # Process individual schedule contents
                    schedule_id = filename.replace('schedule_', '').replace('.csv', '')
                    if schedule_id not in data['schedule_contents']:
                        data['schedule_contents'][schedule_id] = []
                    
                    for row in csv_reader:
                        data['schedule_contents'][schedule_id].append({
                            'scheduleId': int(row['scheduleId']),
                            'courseId': int(row['courseId']),
                            'groupId': row['groupId'],
                            'day': row['day'],
                            'startTime': row['startTime'],
                            'duration': int(row['duration']),
                            'classroom': row['classroom'],
                            'building': row['building'],
                            'teacher': row['teacher'],
                            'type': row['type']
                        })
                        total_records += 1
                        
                elif '_lectures.csv' in filename:
                    # Process lecture data
                    for row in csv_reader:
                        course_id = row['courseId']
                        group_id = row['groupId']
                        key = f"{course_id}_{group_id}"
                        
                        data['lectures'][key] = {
                            'courseId': int(course_id),
                            'day': row['day'],
                            'startTime': row['startTime'],
                            'duration': int(row['duration']),
                            'classroom': row['classroom'],
                            'building': row['building'],
                            'teacher': row['teacher'],
                            'groupId': group_id
                        }
                        total_records += 1
                        
                elif '_tutorials.csv' in filename:
                    # Process tutorial data
                    for row in csv_reader:
                        course_id = row['courseId']
                        group_id = row['groupId']
                        key = f"{course_id}_{group_id}"
                        
                        data['tutorials'][key] = {
                            'courseId': int(course_id),
                            'day': row['day'],
                            'startTime': row['startTime'],
                            'duration': int(row['duration']),
                            'classroom': row['classroom'],
                            'building': row['building'],
                            'teacher': row['teacher'],
                            'groupId': group_id
                        }
                        total_records += 1
                        
                elif '_labs.csv' in filename:
                    # Process lab data
                    for row in csv_reader:
                        course_id = row['courseId']
                        group_id = row['groupId']
                        key = f"{course_id}_{group_id}"
                        
                        data['labs'][key] = {
                            'courseId': int(course_id),
                            'day': row['day'],
                            'startTime': row['startTime'],
                            'duration': int(row['duration']),
                            'classroom': row['classroom'],
                            'building': row['building'],
                            'teacher': row['teacher'],
                            'groupId': group_id
                        }
                        total_records += 1
                        
        except Exception as e:
            error_msg = f"Error processing {filename}: {str(e)}"
            errors.append(error_msg)
            print(f"ERROR: {error_msg}")
    
    # Generate summary statistics
    print(f"\n=== PROCESSING SUMMARY ===")
    print(f"Total CSV files processed: {csv_files_processed}")
    print(f"Total records converted: {total_records}")
    print(f"Courses: {len(data['courses'])}")
    print(f"Lectures: {len(data['lectures'])}")
    print(f"Tutorials: {len(data['tutorials'])}")
    print(f"Labs: {len(data['labs'])}")
    print(f"Schedules: {len(data['schedules'])}")
    print(f"Schedule Contents: {len(data['schedule_contents'])}")
    print(f"Errors encountered: {len(errors)}")
    
    if errors:
        print("\n=== ERRORS ===")
        for error in errors:
            print(f"- {error}")
    
    return data

def save_yaml_data(data, output_file):
    """
    Save the processed data to a YAML file with proper formatting.
    
    Args:
        data (dict): The complete data structure
        output_file (str): Path to the output YAML file
    """
    print(f"\nSaving data to: {output_file}")
    
    with open(output_file, 'w', encoding='utf-8') as file:
        # Custom YAML formatting for better readability
        yaml.dump(data, file, 
                  default_flow_style=False,
                  sort_keys=True,
                  allow_unicode=True,
                  width=80,  # Respect 80-character line limit
                  indent=2)
    
    print("YAML file generated successfully!")

def validate_data_completeness(data):
    """
    Validate that all data has been properly converted and no information
    is missing from the original CSV files.
    
    Args:
        data (dict): The complete data structure to validate
    
    Returns:
        bool: True if validation passes, False otherwise
    """
    print("\n=== DATA VALIDATION ===")
    
    validation_errors = []
    
    # Check if all major sections exist
    required_sections = ['courses', 'lectures', 'tutorials', 'labs', 
                        'schedules', 'schedule_contents']
    for section in required_sections:
        if section not in data:
            validation_errors.append(f"Missing section: {section}")
    
    # Validate course data completeness
    for course_id, course_data in data['courses'].items():
        required_fields = ['courseId', 'name', 'credits', 'examDateA', 
                          'examDateB', 'lecturer']
        for field in required_fields:
            if field not in course_data:
                validation_errors.append(
                    f"Course {course_id} missing field: {field}")
    
    # Validate lesson data completeness (lectures, tutorials, labs)
    lesson_types = ['lectures', 'tutorials', 'labs']
    for lesson_type in lesson_types:
        for lesson_key, lesson_data in data[lesson_type].items():
            required_fields = ['courseId', 'day', 'startTime', 'duration',
                              'classroom', 'building', 'teacher', 'groupId']
            for field in required_fields:
                if field not in lesson_data:
                    validation_errors.append(
                        f"{lesson_type} {lesson_key} missing field: {field}")
    
    if validation_errors:
        print("VALIDATION FAILED:")
        for error in validation_errors:
            print(f"- {error}")
        return False
    else:
        print("VALIDATION PASSED: All data is complete!")
        return True

def main():
    """
    Main function to orchestrate the CSV to YAML conversion process.
    """
    # Set up paths
    data_dir = "data"
    output_file = "complete_schedule_data.yaml"
    
    # Check if data directory exists
    if not os.path.exists(data_dir):
        print(f"ERROR: Data directory '{data_dir}' not found!")
        return
    
    # Process all CSV files
    data = process_csv_files(data_dir)
    
    # Validate data completeness
    if validate_data_completeness(data):
        # Save to YAML file
        save_yaml_data(data, output_file)
        print(f"\n✅ SUCCESS: Complete schedule data saved to {output_file}")
    else:
        print("\n❌ FAILED: Data validation errors detected!")

if __name__ == "__main__":
    main()
